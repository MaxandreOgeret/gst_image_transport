#include "gst_image_transport/h264_subscriber.h"

namespace h264_image_transport
{
void H264Subscriber::subscribeImpl(ros::NodeHandle& nh, const std::string& base_topic, uint32_t queue_size,
                                   const Callback& callback, const ros::VoidPtr& tracked_object,
                                   const image_transport::TransportHints& transport_hints)
{
  queue_size = 10;
  SimpleSubscriberPlugin::subscribeImpl(nh, base_topic, queue_size, callback, tracked_object, transport_hints);
}

void H264Subscriber::internalCallback(const sensor_msgs::CompressedImage::ConstPtr& message, const Callback& user_cb)
{
  decoder_->push_data(message->data);

  if (!decoder_->pull_data()) {
    return;
  }

  out_->data = decoder_->get_data();
  out_->width = decoder_->getWidth();
  out_->height = decoder_->getHeight();
  out_->step = 3 * decoder_->getWidth();
  out_->encoding = sensor_msgs::image_encodings::BGR8;
  out_->header = message->header;
  user_cb(out_);
}
}  // namespace h264_image_transport

PLUGINLIB_EXPORT_CLASS(h264_image_transport::H264Subscriber, image_transport::SubscriberPlugin);