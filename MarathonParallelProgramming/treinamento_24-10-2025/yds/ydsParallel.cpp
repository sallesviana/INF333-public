#include<vector>
#include<set>
#include<iostream>
#include<cmath>
#define ALPHA 3
using namespace std;

struct job {
    int r, d, w;
};
struct interval {
    int r, d, time;
    double dens;
    mutable vector<job>jobs_inside;
    bool operator<(const interval& inter) const {
        if(r < inter.r) return true;
        if(r == inter.r && d < inter.d) return true; 
        return false;
    }
};

const double energy_consumption(const vector<interval>& sch){
    double energy = 0.0;
    for(size_t i = 0; i < sch.size(); i++){
        double dens = sch[i].dens;
        int inter = sch[i].d - sch[i].r;
        energy += pow(dens, ALPHA)*inter;
    }
    return energy;
}


#include <list>
set<interval> build_intervals(const list<job> &jobs2){
    set<interval> intervals;

    vector<job> jobs(jobs2.begin(), jobs2.end());

    set<int> ds; 
    for(auto &[r,d,w]:jobs2) {
        ds.insert(d);
    }
    for(auto &jb: jobs2){
        set<int>::iterator it = ds.find(jb.r);
        it++;
        if(it==ds.end()) continue;
        interval inter {jb.r, jb.d, jb.d - jb.r, 0.0};
        intervals.insert(inter);
    }
    return intervals;

    /*

    for(size_t i = 0; i < jobs.size(); i++){
        for(size_t j = 0; j < jobs.size(); j++){
            if(jobs[i].r >= jobs[j].d) continue; //se r for menor que um dos outros d --> coloca?
            interval inter {jobs[i].r, jobs[i].d, jobs[i].d - jobs[i].r, 0.0};
            intervals.insert(inter);
        }
    }
    return intervals;*/
}



interval find_maximum_density_interval(const list<job>& jobs){
    set<interval> intervals = build_intervals(jobs);
    double max_dens = -1.0;
    interval max_interval;
    for(set<interval>::iterator it = intervals.begin(); it!= intervals.end(); ++it){
        int inter_r = it->r, inter_d = it->d;
        double dens = 0.0;
        //for(size_t i = 0; i < jobs.size(); i++){
        for(auto &jb:jobs) {
            if(jb.r >= inter_r && jb.d <= inter_d){
                it->jobs_inside.push_back(jb);
                dens += jb.w;
            }
        }
        dens /= it->time;
        if(dens >= max_dens) {
            max_dens = dens;
            max_interval = *it;
            max_interval.dens = dens;
        }
    }
    return max_interval;
}

void remove_jobs_from_interval(list<job>& jobs, const interval inter){
    //for(size_t i = 0; i < jobs.size(); i++){
    for(list<job>::iterator it = jobs.begin(); it!=jobs.end(); ) {
        auto &jb = *it;

        int inter_r = inter.r, inter_d = inter.d;
        if(jb.r >= inter_r && jb.d <= inter_d){
            it = jobs.erase(it); //jobs.begin() + i);
           // i--;
        } else it++;
    }
}

void adjust_jobs_given_interval(list<job>& jobs, const interval inter){
    //for(size_t i = 0; i < jobs.size(); i++){
    for(auto &jb:jobs) {
        int inter_r = inter.r, inter_d = inter.d;
        if(jb.r >= inter_r && jb.r <= inter_d && jb.d >= inter_d){
            jb.r = inter_d;
        } else if(jb.r <= inter_r && jb.d >= inter_r && jb.d <= inter_d){
            jb.d = inter_r;
        }
    }
}   

vector<interval> yds(vector<job> &jobs2){
    list<job> jobs( jobs2.begin(), jobs2.end() );
    vector<interval> schedule;
    while(!jobs.empty()) {
        // find maximum density interval
        interval max_interval = find_maximum_density_interval(jobs);
        
        // put max density interval on the schedule
        schedule.push_back(max_interval);

        // remove max_interval jobs from the total jobs
        remove_jobs_from_interval(jobs, max_interval);
        
        // adjust jobs
        adjust_jobs_given_interval(jobs, max_interval);
    }
    return schedule;
}

int main(){
    int n;
    vector<job>jobs;
    
    cin >> n;
    for(int k = 0; k < n; k++){
        int r, d, w;
        cin >> r >> d >> w;
        job j {r, d, w};
        jobs.push_back(j);
    }
    
    vector<interval> schedule = yds(jobs);     
    cout << energy_consumption(schedule) << endl;
    return 0;
}


