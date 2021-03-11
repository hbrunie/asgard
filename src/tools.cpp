#include "tools.hpp"
#include <algorithm>
// FIXME use string format after C++20
// #include <format>
#include <math.h>
#include <numeric>

namespace tools
{
std::string simple_timer::report()
{
  std::ostringstream report;
  report << "\nperformance report, all times in ms...\n\n";

  for (auto [id, times] : id_to_times_)
  {
    auto const avg =
        std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    auto const min = *std::min_element(times.begin(), times.end());
    auto const max = *std::max_element(times.begin(), times.end());

    // calculate median
    auto const middle_it = times.begin() + times.size() / 2;
    std::nth_element(times.begin(), middle_it, times.end());
    auto const med =
        times.size() % 2 == 0
            ? (*std::max_element(times.begin(), middle_it) + *middle_it) / 2
            : *middle_it;

    auto const avg_flops = [this, id = id]() {
      if (id_to_flops_.count(id) > 0)
      {
        auto const flops = id_to_flops_[id];
        auto const sum   = std::accumulate(flops.begin(), flops.end(), 0.0);

        if (isinf(sum))
        {
          return std::string(" avg gflops: inf");
        }
        auto const avg = sum / flops.size();
        return std::string(" avg gflops: ") + std::to_string(avg);
      }
      return std::string("");
    }();

    report << id << " - avg: " << avg << " min: " << min << " max: " << max
           << " med: " << med << avg_flops << " calls: " << times.size()
           << '\n';
  }
  return report.str();
}

simple_timer timer;

#ifdef ASGARD_USE_SHAMAN
template<>
std::string vec2csv(const std::vector<Sfloat>& vec)
{
    std::ostringstream s;
    s << std::scientific;// << std::setprecision(16);

  for(unsigned long i = 0; i < vec.size(); i++)
  {
      const Sfloat x = vec[i];
      std::string dig =((int)x.digits()) < -1 ? "all (zero)" : std::to_string((int)x.digits());
    s << '(' << i << ") "
      << x << '|'
      //<< x.number << '|'
      //<< x.error  << '|'
      << dig << std::endl;
    // why not use Shaman's default streaming operator instead of the above?
   //s << '(' << i << ") " <<x;
    if (i+1 < vec.size())
        {
            s << ", ";
        }
  }
  return s.str();
}
template<>
std::string vec2csv(const std::vector<Sdouble>& vec)
{
    std::ostringstream s;
    s << std::scientific;// << std::setprecision(16);

  for(unsigned long i = 0; i < vec.size(); i++)
  {
      const Sdouble x = vec[i];
      std::string dig =((int)x.digits()) < -1 ? "all (zero)" : std::to_string((int)x.digits());
    s << '(' << i << ") "
      << x << '|'
      //<< x.number << '|'
      //<< x.error  << '|';
      << dig << std::endl;
    // why not use Shaman's default streaming operator instead of the above?
   //s << '(' << i << ") " <<x;
    if (i+1 < vec.size())
        {
            s << ", ";
        }
  }
  return s.str();
}
#endif
} // namespace tools
