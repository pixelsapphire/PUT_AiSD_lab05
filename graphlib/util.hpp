#ifndef AISD_UTIL_HPP
#define AISD_UTIL_HPP

#include <chrono>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace gr {

    template<typename _Tp>
    std::string vec_str(const std::vector<_Tp>& __vec) {
        std::stringstream ss;
        for (const auto& x : __vec) ss << x << " ";
        return ss.str();
    }

    template<typename Dur, typename std::enable_if<std::chrono::__is_duration<Dur>::value>::type* = nullptr>
    class timer {

        std::chrono::time_point<std::chrono::high_resolution_clock, Dur> start_point, end_point;
        bool running = false;

    public:

        timer() {
            start_point = std::chrono::high_resolution_clock::now();
            end_point = std::chrono::high_resolution_clock::now();
            running = false;
        }

        std::chrono::time_point<std::chrono::high_resolution_clock, Dur> now() const {
            return std::chrono::high_resolution_clock::now();
        }

        void start() {
            if (!running) {
                running = true;
                start_point = now();
            } else throw std::runtime_error("Illegal timer state: timer is already running");
        }

        void stop() {
            if (running) {
                end_point = now();
                running = false;
            } else throw std::runtime_error("Illegal timer state: timer is not running");
        }

        int get_measured_time() {
            Dur time = std::chrono::duration_cast<Dur>(end_point - start_point);
            return time.count();
        }

        std::string unit() {
            constexpr intmax_t denominator = Dur::period::den;
            if (denominator == 1) return "s";
            else if (denominator == 1000) return "ms";
            else if (denominator == 1000000) return "us";
            else if (denominator == 1000000000) return "ns";
            return "?";
        }
    };

    typedef std::chrono::duration<double, std::nano> precision_ns;
    typedef std::chrono::duration<double, std::micro> precision_us;
    typedef std::chrono::duration<double, std::milli> precision_ms;
    typedef std::chrono::duration<double, std::ratio<1, 1>> precision_s;

#define scan(collection) (collection).begin(), (collection).end()
}

#endif //AISD_UTIL_HPP
