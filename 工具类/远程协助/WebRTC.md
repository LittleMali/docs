
WebRTC使用了SDL做跨平台的音视频和键鼠消息处理。

SDL的实现：
peerconnectionStream.h

void sendVideoBuffer(const VideoFrame& videoFrame, const char* channelId);
void sendAudioBuffer(const AudioFrame& audioFrame, const char* channelId);

SDL又调回了WebRTC lib，头文件的实现是在lib里面，或者说是chromium里面。
如何捕获屏幕，看这个地方。
DesktopCapturer::CreateScreenCapturer