// sample: https://atcoder.jp/contests/future-contest-2023-final/submissions/37130921

#include <bits/stdc++.h>
// #include "atcoder/all"


#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

using namespace std;

using i64 = long long;

constexpr int n = 50;

const i64 MOD = 1e9 + 7;
const i64 INF = i64(1e18);

template <typename T>
bool chmin(T& x, T y){
    if(x > y){
        x = y;
        return true;
    }
    return false;
}

template <typename T>
bool chmax(T& x, T y){
    if(x < y){
        x = y;
        return true;
    }
    return false;
}

uint64_t rnd(){
    static uint64_t x = 0xdeadbeef0110dead;
    x ^= x << 7;
    x ^= x >> 9;
    return x;
}

double rnd_double(){
    return 1.0 * rnd() / numeric_limits<uint64_t>::max();
}

double rnd_norm(){
    auto x = rnd_double();
    auto y = rnd_double();
    return sqrt(-2 * log(x)) * cos(2 * M_PI * y);
}

// #define OPTIMIZE

namespace params{
void load_params(){
    ifstream ifs("../params.txt");
    assert(ifs.is_open());
}

}

i64 dist(int px, int py, int qx, int qy){
    return 1LL * (px - qx) * (px - qx) + 1LL * (py - qy) * (py - qy);
}

struct Log{
    int x, y;
    int selected;
    i64 dist;
    double diag;
    bool ok;
};

struct Env{
    double sigma;
    vector<int> ok_idxes;
    vector<pair<int,int>> data;
    vector<pair<int,int>> know;
    vector<int> unknown;
    vector<Log> log;
    int last_ok = -1;

    void init(double _sigma, vector<pair<int,int>> v = vector<pair<int,int>>()){
        sigma = _sigma;
        for(int i = 0; i < n; ++i){
            ok_idxes.emplace_back(i);
        }
        data = move(v);
    }
    void clear_uk(int x, int y){
        vector<int> new_uk;
        for(auto i : unknown){
            if(dist(log[i].x, log[i].y, x, y) >= 5e15){
                new_uk.emplace_back(i);
            }
        }
        unknown = move(new_uk);
    }
    void sample_rot(int x, int y, bool _log = true){
        assert(data.size() == n);

        auto idx = [&](){
            vector<double> weights;
            for(auto i : ok_idxes){
                i64 d = max(1e-9, 1.0 * dist(x, y, data[i].first, data[i].second));
                weights.emplace_back(1.0 / d);
            }
            double weight_sum = accumulate(weights.begin(), weights.end(), 0.0);
            for(auto &w : weights){
                w /= weight_sum;
            }

            /*
            auto w2 = weights;
            sort(w2.begin(), w2.end(), greater<>());
            for(int i = 0; i < 5; ++i){
                cout << w2[i] << " \n"[i == 4];
            }
             */

            double prob = rnd_double();
            double now = 0;
            for(int i = 0; i < n; ++i){
                now += weights[i];
                if(prob <= now){
                    return ok_idxes[i];
                }
            }
            return ok_idxes.back();
        }();
        int k = idx;
        auto [px, py] = data[k];
        double diag = atan2(py - y, px - x);
        diag += rnd_norm() * sigma;
        diag += 4 * M_PI;
        while(diag > 2 * M_PI){
            diag -= 2 * M_PI;
        }
        // TODO add sigma to diag
        bool ok = false;
        auto dist_ = dist(x, y, px, py);
        if(dist_ <= 1e12){
            ok = true;
            last_ok = log.size();
            vector<int> new_ok;
            for(auto kk : ok_idxes){
                if(kk != k){
                    new_ok.emplace_back(kk);
                }
            }
            ok_idxes = new_ok;
            clear_uk(px, py);
            know.emplace_back(px, py);
        }
        else{
            unknown.emplace_back(log.size());
        };
        if(_log){
            log.emplace_back(Log{x, y, k, dist_, diag, ok});
        }
    }
    void write(ofstream& ofs_stdout){
        for(auto log_ : log){
            ofs_stdout << log_.x << " " << log_.y << endl;
            ofs_stdout << "#r " << log_.ok << " " << log_.diag << endl;
        }
    }
};


Env env;

pair<int,int> calc_next(){
    /*
    if(env.log.size() - env.last_ok >= 20){
        cout << "#reset by 20 turns" << endl;
        env.last_ok = env.log.size() - 1;
    }
     */

    int failed_cnt = 0;
    vector<int> target_log;
    for(int i = env.log.size() - 1; i >= 0; --i){
        if(env.log[i].ok || i == env.last_ok){
            break;
        }
        target_log.emplace_back(i);
        ++failed_cnt;
    }
    auto origin_log = target_log;
    int siz = target_log.size();
    cout << "# challenge: " << env.log.size() - env.last_ok - 1 << endl;
    cout << "# collect: " << 50 - env.ok_idxes.size() << endl;

    /* TODO: unknown
    if(env.ok_idxes.size() <= 10){
        for(auto i : env.unknown){
            if(dist(i, i, ))
        }
    }
     */

    if(failed_cnt == 0){
        int x = 1e9;
        int y = 1e9;
        auto best = make_tuple(0LL, x, y);
        for(int iter = 0; iter < 10000; ++iter){
            x = int(rnd_double() * 2e9 - 1e9);
            y = int(rnd_double() * 2e9 - 1e9);
            if(dist(0, 0, x, y) >= 6e17){
                --iter;
                continue;
            }
            i64 sc = numeric_limits<i64>::max();
            for(auto& log : env.log){
                chmin(sc, dist(x, y, log.x, log.y));
            }
            chmax(best, make_tuple(sc, x, y));
        }
        return {get<1>(best), get<2>(best)};
    }
    else if(failed_cnt < 4){
        vector<double> da;
        for(auto i : target_log){
            da.emplace_back(env.log[i].diag);
        }
        auto mi = *min_element(da.begin(), da.end());
        auto ma = *max_element(da.begin(), da.end());
        if(ma - mi <= M_PI / 2){
            auto &last = env.log.back();
            double diag = last.diag + 0.5 * rnd_double();
            double go = 1e9 * (0.25 - failed_cnt * 0.05);
            int x = last.x + cos(diag) * go;
            int y = last.y + sin(diag) * go;
            if(dist(0, 0, x, y) <= 1e18){
                return {x, y};
            }
        }
    }
    int del_cnt = failed_cnt < 4 ? 0 : (failed_cnt < 10 ? 2 : failed_cnt < 15 ? 3 : 4);
    for(int i = 0; i < del_cnt; ++i){
        target_log.pop_back();
    }

    vector<int> target_back5;
    for(auto i : target_log){
        target_back5.emplace_back(i);
        if(target_back5.size() >= 6){
            break;
        }
    }

    auto calc_prob = [&](int x, int y, const vector<int>& log_idxes){
        vector<double> per_vec;

        for(auto i : log_idxes){
            int qx = env.log[i].x;
            int qy = env.log[i].y;
            i64 dist_ = dist(qx, qy, x, y);

            double diag = atan2(y - qy, x - qx);
            double diag_diff = abs(diag - env.log[i].diag);
            while(diag_diff >= 2 * M_PI){
                diag_diff -= 2 * M_PI;
            }
            if(diag_diff >= M_PI){
                diag_diff = 2 * M_PI - diag_diff;
            }
            assert(-M_PI <= diag_diff && diag_diff <= M_PI);

            double correct_per;
            // correct per
            {
                // TODO: 5.0
                double gosa_norm = diag_diff / 0.4;
                // 1: 0.34, 2: 0.477
                double prob_cumsum = 0.5 * erf(gosa_norm / M_SQRT2);
                // 1: 1 - 0.68
                correct_per = min(1.0, max(0.0, 1.0 - 2 * prob_cumsum));
            }
            // incorrect per
            double per = correct_per;
            per_vec.emplace_back(per);
        }

        double arg = atan2(
                y - env.log.back().y,
                x - env.log.back().x);
        while(arg < 0){
            arg += 2 * M_PI;
        }
        /*
        double argmin = min(arg, env.log.back().diag);
        double argmax = max(arg, env.log.back().diag);
        double arg_diff = min(argmax - argmin, 2 * M_PI - (argmin - argmax));
        assert(0 <= arg_diff && arg_diff <= 2 * M_PI);
         */

        double per_sum = accumulate(per_vec.begin(), per_vec.end(), 0.0, [](auto x, auto y){return x + y;});

        // auto rot = 100e18;
        // double score_pena = max(0.0, (dist(x, y, env.log.back().x, env.log.back().y) - rot) / rot);

        double per_avg = per_sum / per_vec.size(); //  - score_pena;
        /*
        if(arg_diff >= M_PI){
            return -1.0;
        }
         */
        return per_avg;
    };
    constexpr double final_move_dist = 1e6;

    pair<int,int> xy = make_pair(
            env.log.back().x + final_move_dist * cos(env.log.back().diag),
            env.log.back().y + final_move_dist * sin(env.log.back().diag)
    );
    double score = calc_prob(xy.first, xy.second, target_log);
    + 1.5 * calc_prob(xy.first, xy.second, target_back5);

    auto best = make_pair(score, xy);
    for(int d = 0; d < 10; ++d){
        double r = rnd_double() * 10e7;
        double d2 = env.log.back().diag + (rnd_double() - 0.5);
        pair<int,int> xy = make_pair(
                env.log.back().x + r * cos(d2),
                env.log.back().y + r * sin(d2)
        );
        double score = calc_prob(xy.first, xy.second, target_log);
        + 1.5 * calc_prob(xy.first, xy.second, target_back5);
        for(int iter = 0; iter < 110; ++iter){
            // TODO: yamanobori using calc_prob !
            double r = 0;
            if(siz >= 10){
                r = rnd_double() * 1e9 * 0.05;
            }
            else{
                r = rnd_double() * 1e9 * 0.15;
            }
            double diag = rnd_double() * 2 * M_PI;
            pair<int,int> nxy = make_pair(xy.first + r * cos(diag), xy.second + r * sin(diag));
            double nex_score = calc_prob(nxy.first, nxy.second, target_log) + 1.5 * calc_prob(nxy.first, nxy.second, target_back5);
            chmax(best, make_pair(nex_score, nxy));
            if(score < nex_score){
                score = nex_score;
                xy = nxy;
            }
        }
    }

    tie(score, xy) = best;

    for(int iter = 0; iter < 1000; ++iter){
        // TODO: yamanobori using calc_prob !
        double r = rnd_double() * 1e8 * 0.7;
        double diag = rnd_double() * 2 * M_PI;
        pair<int,int> nxy = make_pair(xy.first + r * cos(diag), xy.second + r * sin(diag));
        double nex_score = calc_prob(nxy.first, nxy.second, target_log) + 1.5 * calc_prob(nxy.first, nxy.second, target_back5);
        chmax(best, make_pair(nex_score, nxy));
        if(score < nex_score){
            score = nex_score;
            xy = nxy;
        }
    }
    xy = best.second;

    /*
    if(target_log.size() >= 10 && calc_prob(xy.first, xy.second, target_back5) <= 1.0){
        // yabai
        cout << "#reset by 10 turn prob: " << calc_prob(xy.first, xy.second, target_back5) << endl;
        env.last_ok = env.log.size();
    }
     */

    // TODO

    cout << "# pA " << calc_prob(xy.first, xy.second, target_log) << endl;
    cout << "# p5 " << calc_prob(xy.first, xy.second, target_back5) << endl;

    auto xyf = xy;
    if(rnd_double() < 0.5){
        double diag = env.log.back().diag - (M_PI / 2);
        double r2 = rnd_double() * 5e7;
        xy = make_pair(
                xyf.first + cos(diag) * r2,
                xyf.second+ sin(diag) * r2);
    }
    if(dist(0, 0, xy.first, xy.second) > 1e18){
        cout << "# OUT OF RANGE" << endl;
        // TODO: maybe it is a bug!
        auto rad = atan2(xy.second, xy.first);
        xy = make_pair(
                (1e9 - 1e7) * cos(rad),
                (1e9 - 1e7) * sin(rad)
        );
    }

    return xy;
}

void solve(){
    for(int i = 0; i < 1000; ++i){
        auto [x, y] = calc_next();
        cout << x << " " << y << endl;
        int r;
        cin >> r;
        if(r == 0){
            double theta;
            cin >> theta;
            env.log.emplace_back(Log{
                    x, y, -1, INF, theta, false
            });
        }
        else{
            int x, y;
            cin >> x >> y;
            if(r == 2){
                break;
            }
            else{
                // TODO?
                env.clear_uk(x, y);
                env.know.emplace_back(x, y);
                env.last_ok = env.log.size();
                env.ok_idxes.pop_back();
                env.log.emplace_back(Log{
                        x, y, -1, 0, 0, true
                });
                env.data.emplace_back(x, y);
            }
        }
    }
}

#ifdef HAND
void simulate(const vector<pair<int,int>>& data){
    while(env.log.size() < 1000){
        if(env.log.size() >= 1000){
            break;
        }
        auto [px, py] = calc_next();
        if(dist(0, 0, px, py) > 1e18){
            break;
        }
        env.sample_rot(px, py, true);
        auto& last = env.log.back();
        cout << "turn: " << env.log.size() << " / " << 1000 << endl;
        // cout << "(" << px << "," << py << ")" << " ==> " << "(" << data[last.selected].first << ", " << data[last.selected].second << ")" << endl;
        cout << "selected: " << last.selected << "\t";
        cout << "dist: " << log10(sqrt(last.dist)) << "\t" << "diag: " << last.diag << "\t" << "ok: " << last.ok << endl;
        cout << endl;
    }
}
#endif

signed main(){

#ifdef OPTIMIZE
    params::load_params();
#endif

    double sigma;

#ifndef HAND
    cin >> sigma;
    env.init(sigma);
    solve();
#else
    ifstream ifs("./tools/in/0000.txt");
    assert(ifs.is_open());
    ifs >> sigma;
    vector<pair<int,int>> data(n);
    for(int i = 0; i < n; ++i){
        ifs >> data[i].first >> data[i].second;
    }
    env.init(sigma, data);
    simulate(data);
    ofstream ofs("./tools/out/0000_stdout.txt");
    assert(ofs.is_open());
    env.write(ofs);
#endif

}
