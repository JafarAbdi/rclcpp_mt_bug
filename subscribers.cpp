#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("test_node");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  std::thread spinner{[&] { executor.spin(); }};
  // Using this version work as expected
  /* std::thread spinner{[&] { */
  /*   while (rclcpp::ok()) */
  /*     executor.spin_once(std::chrono::milliseconds(100)); */
  /* }}; */

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub1;
  {
    auto callback_group = node->create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);
    auto subscription_options = rclcpp::SubscriptionOptions();
    subscription_options.callback_group = callback_group;
    sub1 = node->create_subscription<std_msgs::msg::String>(
        "topic", 5,
        [](std_msgs::msg::String::UniquePtr msg) {
          RCLCPP_ERROR_STREAM(rclcpp::get_logger("sub1"),
                              "Message: " << msg->data);
        },
        subscription_options);
  }
  std::this_thread::sleep_for(std::chrono::seconds(5));
  RCLCPP_ERROR_STREAM(rclcpp::get_logger("testing"), "First 5s");
  // If I comment one line of the following it work as expected
  sub1.reset();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  RCLCPP_ERROR_STREAM(rclcpp::get_logger("testing"), "Second 5s");
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub2;
  {
    auto callback_group = node->create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);
    auto subscription_options = rclcpp::SubscriptionOptions();
    subscription_options.callback_group = callback_group;
    sub2 = node->create_subscription<std_msgs::msg::String>(
        "topic", 5,
        [](std_msgs::msg::String::UniquePtr msg) {
          RCLCPP_ERROR_STREAM(rclcpp::get_logger("sub2"),
                              "Message: " << msg->data);
        },
        subscription_options);
  }
  spinner.join();
  rclcpp::shutdown();
  return 0;
}
