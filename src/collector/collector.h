/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>  // NOLINT
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace examples {
namespace collector {

class Collector final : public roq::Client {
 public:
  explicit Collector(
      roq::Client::Dispatcher& dispatcher,
      const std::string& output_file);
  ~Collector();

  struct State final {
    explicit State(const std::string& symbol)
        : symbol(symbol) {}
    void update(const roq::SessionStatisticsEvent&);
    void update(const roq::DailyStatisticsEvent&);
    void update(const roq::MarketByPriceEvent&);
    void update(const roq::TradeSummaryEvent&);
    std::string symbol;
    roq::time_point_t exchange_time;
    roq::time_point_t receive_time;
    roq::Layer depth[5] = {};
    double price = 0.0;
    double volume = 0.0;
    double turnover = 0.0;
    double pre_open_interest = 0.0;
    double pre_settlement_price = 0.0;
    double pre_close_price = 0.0;
    double highest_traded_price = 0.0;
    double lowest_traded_price = 0.0;
    double upper_limit_price = 0.0;
    double lower_limit_price = 0.0;
    double open_interest = 0.0;
    double open_price = 0.0;
  };

 protected:
  const roq::Subscriptions& get_subscriptions() const override;
  void on(const roq::TimerEvent&) override {}
  void on(const roq::ConnectionStatusEvent&) override {}
  void on(const roq::BatchBeginEvent&) override {}
  void on(const roq::BatchEndEvent&) override;
  void on(const roq::MarketDataStatusEvent&) override {}
  void on(const roq::OrderManagerStatusEvent&) override {}
  void on(const roq::DownloadBeginEvent&) override {}
  void on(const roq::DownloadEndEvent&) override {}
  void on(const roq::ReferenceDataEvent&) override {}
  void on(const roq::MarketStatusEvent&) override {}
  void on(const roq::SessionStatisticsEvent&) override;
  void on(const roq::DailyStatisticsEvent&) override;
  void on(const roq::MarketByPriceEvent&) override;
  void on(const roq::TradeSummaryEvent&) override;
  void on(const roq::CreateOrderAckEvent&) override {}
  void on(const roq::ModifyOrderAckEvent&) override {}
  void on(const roq::CancelOrderAckEvent&) override {}
  void on(const roq::OrderUpdateEvent&) override {}
  void on(const roq::TradeUpdateEvent&) override {}
  void on(const roq::PositionUpdateEvent&) override {}

  State& get(const std::string& symbol);

 private:
  roq::Client::Dispatcher& _dispatcher;
  std::ofstream _output_file;
  bool _use_output_file = false;
  std::unordered_map<std::string, State> _cache;
  std::unordered_set<State*> _dirty;
};

std::ostream& operator<<(std::ostream&, Collector::State&);

}  // namespace collector
}  // namespace examples
