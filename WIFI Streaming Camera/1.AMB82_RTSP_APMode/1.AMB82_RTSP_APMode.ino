/////////////////////////////////////////////////////////////////
/*
  WiFi Streaming Camera for AMB32-Mini
  For More Information: https://youtu.be/FEs1RIjfO3Y
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////

#include "WiFi.h"
#include "StreamIO.h"
#include "VideoStream.h"
#include "AudioStream.h"
#include "AudioEncoder.h"
#include "RTSP.h"

#define CHANNEL 1

VideoSetting configV(CHANNEL);
AudioSetting configA(0);
Audio audio;
AAC aac;
RTSP rtsp;
StreamIO audioStreamer(1, 1);  // 1 Input Audio -> 1 Output AAC
StreamIO avMixStreamer(2, 1);  // 2 Input Video + Audio -> 1 Output RTSP

char ssid[] = "AMB82-Mini";  // your network SSID (name)
char pass[] = "Password";    // your network password

int status = WL_IDLE_STATUS;
char channel[] = "1";  // Set the AP channel
int ssid_status = 0;

void setup() {
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to start AP with SSID: ");
    Serial.println(ssid);
    status = WiFi.apbegin(ssid, pass, channel, ssid_status);
    // wait 2 seconds for connection:
    delay(2000);
  }

  // Configure camera video channel with video format information
  // Adjust the bitrate based on your WiFi network quality
  configV.setBitrate(2 * 1024 * 1024);  // Recommend to use 2Mbps for RTSP streaming to prevent network congestion
  Camera.configVideoChannel(CHANNEL, configV);
  Camera.videoInit();

  // Configure audio peripheral for audio data output
  audio.configAudio(configA);
  audio.begin();
  // Configure AAC audio encoder
  aac.configAudio(configA);
  aac.begin();

  // Configure RTSP with identical video format information and enable audio streaming
  rtsp.configVideo(configV);
  rtsp.configAudio(configA, CODEC_AAC);
  rtsp.begin();

  // Configure StreamIO object to stream data from audio channel to AAC encoder
  audioStreamer.registerInput(audio);
  audioStreamer.registerOutput(aac);
  if (audioStreamer.begin() != 0) {
    Serial.println("StreamIO link start failed");
  }

  // Configure StreamIO object to stream data from video channel and AAC encoder to rtsp output
  avMixStreamer.registerInput1(Camera.getStream(CHANNEL));
  avMixStreamer.registerInput2(aac);
  avMixStreamer.registerOutput(rtsp);
  if (avMixStreamer.begin() != 0) {
    Serial.println("StreamIO link start failed");
  }

  // Start data stream from video channel
  Camera.channelBegin(CHANNEL);

  delay(1000);
  printInfo();
}

void loop() {
  // Do nothing
}

void printInfo(void) {
  Serial.println("------------------------------");
  Serial.println("- Summary of Streaming -");
  Serial.println("------------------------------");
  Camera.printInfo();

  IPAddress ip = WiFi.localIP();

  Serial.println("- RTSP -");
  Serial.print("rtsp://");
  Serial.print(ip);
  Serial.print(":");
  rtsp.printInfo();

  Serial.println("- Audio -");
  audio.printInfo();
}
