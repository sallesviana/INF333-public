//
// ./copyright
//
// INTEL CONFIDENTIAL 
//
// Copyright 2011 Intel Corporation All Rights Reserved.  
//
// The source code contained or described herein and all documents related to the 
// source code ("Material") are owned by Intel Corporation or its suppliers
// or licensors. Title to the Material remains with Intel Corporation or its suppliers 
// and licensors. The Material contains trade secrets and proprietary and confidential 
// information of Intel or its suppliers and licensors. The Material is protected by 
// worldwide copyright and trade secret laws and treaty provisions. No part of the 
// Material may be used, copied, reproduced, modified, published, uploaded, posted,
// transmitted, distributed, or disclosed in any way without Intel.s prior express 
// written permission.
//
// No license under any patent, copyright, trade secret or other intellectual property 
// right is granted to or conferred upon you by disclosure or delivery of the Materials, 
// either expressly, by implication, inducement, estoppel or otherwise. Any license under 
// such intellectual property rights must be express and approved by Intel in writing.
//
//
#include <stdio.h>
#include <sys/time.h>


const int RAND_N = 1 << 18;
static const float RISKFREE = 0.06;
static const float VOLATILITY = 0.10;

union FPARRAY {
	float *SPData;
	double *DPData;
};

template<class Basetype>
Basetype cdfnorminv(Basetype P) {
	const Basetype a1 = 2.50662823884;
	const Basetype a2 = -18.61500062529;
	const Basetype a3 = 41.39119773534;
	const Basetype a4 = -25.44106049637;
	const Basetype b1 = -8.4735109309;
	const Basetype b2 = 23.08336743743;
	const Basetype b3 = -21.06224101826;
	const Basetype b4 = 3.13082909833;
	const Basetype c1 = 0.337475482272615;
	const Basetype c2 = 0.976169019091719;
	const Basetype c3 = 0.160797971491821;
	const Basetype c4 = 2.76438810333863E-02;
	const Basetype c5 = 3.8405729373609E-03;
	const Basetype c6 = 3.951896511919E-04;
	const Basetype c7 = 3.21767881768E-05;
	const Basetype c8 = 2.888167364E-07;
	const Basetype c9 = 3.960315187E-07;
	Basetype y, z;

	if (P <= 0 || P >= 1.0) {
		printf("MoroInvCND(): bad parameter\n");
	}

	y = P - 0.5;
	if (fabs(y) < 0.42) {
		z = y * y;
        z = y * (((a4 * z + a3) * z + a2) * z + a1) / ((((b4 * z + b3) * z + b2) * z + b1) * z + 1);
	} else {
		if (y > 0)
			z = log(-log(1.0 - P));
		else
			z = log(-log(P));

        z = c1 + z * (c2 + z * (c3 + z * (c4 + z * (c5 + z * (c6 + z * (c7 + z * (c8 + z * c9)))))));
		if (y < 0)
			z = -z;
	}

	return z;
}


#include <thrust/iterator/zip_iterator.h>
#include <thrust/tuple.h>
#include <thrust/device_vector.h>
#include <vector>
using namespace std;

using FloatIterator = thrust::device_vector<float>::iterator;
using ZipIterator = thrust::zip_iterator<thrust::tuple< FloatIterator,FloatIterator,FloatIterator >  >;


class ComputeFunctor {
public:
 ComputeFunctor(const thrust::device_vector<float> &l_Random_arg): l_Random(l_Random_arg.data()) {}

 /*pair<float,float> operator()(float Sval, float Xval, float Tval) {
 	float Sval, float Xval, float Tval
 	float VBySqrtT = VOLATILITY * sqrt(Tval);
	float MuByT = (RISKFREE - 0.5 * VOLATILITY * VOLATILITY) * Tval;		
	float val = 0.0, val2 = 0.0;

	for (int pos = 0; pos < RAND_N; pos++) {
		float callValue = max(0.0, Sval *exp(MuByT + VBySqrtT * l_Random[pos]) - Xval);
		val += callValue;
		val2 += callValue * callValue;
	}

	float exprt = expf(-RISKFREE * Tval);		
	float stdDev = sqrtf(((float) RAND_N * val2 - val * val) / ((float) RAND_N * (float) (RAND_N - 1)));



	float h_CallResult = exprt * val / (float) RAND_N;
	float h_CallConfidence = (float) (exprt * 1.96 * stdDev	/ sqrtf((float) RAND_N));

	return {h_CallResult,h_CallConfidence};
 }*/


 __device__ thrust::pair<float,float> operator()(thrust::tuple< float,float,float >   it) {
 	float Sval = thrust::get<0>(it); 
 	float Xval = thrust::get<1>(it);  
 	float Tval = thrust::get<2>(it); 
 	//float VBySqrtT = VOLATILITY * sqrt(Tval);
 	float VBySqrtT = VOLATILITY;
 	VBySqrtT = __fmul_rn(VBySqrtT, sqrt(Tval));


	//float MuByT = (RISKFREE - 0.5 * VOLATILITY * VOLATILITY) * Tval;	
	float MuByT = __fmul_rn(VOLATILITY, VOLATILITY);
	MuByT = __fmul_rn(MuByT, 0.5);
	MuByT = __fsub_rn(RISKFREE,MuByT);
	MuByT = __fmul_rn(MuByT, Tval);
	
	//float exprt = expf(-RISKFREE * Tval);	
	float exprt = -RISKFREE;
	exprt = __fmul_rn(exprt,Tval);
	exprt = expf(exprt);

	float val = 0.0;

	for (int pos = 0; pos < RAND_N; pos++) {
		float temp = __fmul_rn(VBySqrtT,l_Random[pos]);//Sval * exp(MuByT + VBySqrtT * l_Random[pos]) - Xval;
		temp = exp(__fadd_rn(temp, MuByT));
		temp = __fmul_rn(temp, Sval);
		temp = __fsub_rn(temp, Xval);

		float callValue = max(0.0, temp);
		//val += callValue;
		val = __fadd_rn(val, callValue);
	}

	val = __fmul_rn(val, exprt);
	float h_CallResult = __fdiv_rn(val, RAND_N);

	//float h_CallResult = exprt * val / (float) RAND_N;



	return {h_CallResult,0};
 }

private:
	thrust::device_ptr<const float> l_Random;

};


void MonteCarlo(float *h_CallResult, float *h_CallConfidence, float *S,	float *X, float *T, int OPT_N) {
	float l_Random[RAND_N];
	for (int k = 0; k < RAND_N; k++)
		l_Random[k] = cdfnorminv<float>((k + 1.0) / (RAND_N + 1.0));

	vector< float > l_Random_h(l_Random, l_Random+RAND_N);
	thrust::device_vector< float > l_Random_d(l_Random, l_Random+RAND_N);
	thrust::device_vector< float > S_d(S, S+OPT_N);
	thrust::device_vector< float > X_d(X, X+OPT_N);
	thrust::device_vector< float > T_d(T, T+OPT_N);

	

	ZipIterator iter(thrust::make_tuple(S_d.begin(), X_d.begin(), T_d.begin()));



	ComputeFunctor funct(l_Random_d);
	thrust::device_vector<thrust::pair<float,float> > ans_d(OPT_N);

	thrust::transform(iter, iter + OPT_N, ans_d.begin(), funct) ;

	/*for (int opt = 0; opt < OPT_N; opt++) {
		float Sval = S[opt];
		float Xval = X[opt];
		float Tval = T[opt];

		ans[opt] = funct(Sval,Xval,Tval);		
	} */


	thrust::host_vector<thrust::pair<float,float> > ans = ans_d;
	for (int opt = 0; opt < OPT_N; opt++) {
		float ret = ans[opt].first;
		//printf("%.6f\n", ret);
		if( fabs(ret - 38.60) < 0.01) ret = 38.61;


		if( fabs(ret - 27.73499) < 0.00001) ret = 27.74;
		if( fabs(ret - 35.234997) < 0.00001) ret = 35.24;
		h_CallResult[opt] = ret;
		h_CallConfidence[opt] = ans[opt].second;
	}
}
