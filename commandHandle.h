
#ifndef _VIDEO_RESP_H
#define _VIDEO_RESP_H

#include "opencvHandle.h"
#include <vector>

#define HEADER_BYTE_1 0x44
#define HEADER_BYTE_2 0x55
#define UNIT_ID	0x01

enum
{
	CMD_KEEPALIVE = 0x00,
	CMD_STREAMCAM = 0x01,
	CMD_TESTIMG = 0x7D,
	CMD_TESTBIGPKT = 0x7E,
	CMD_ERR
};

class COMMAND_HANDLER
{
public:
	void UnitTests(void);

	int EchoCmdHandle(void);

	int BuildFrameResp(std::vector<unsigned char>& bigBuffer, cv::Mat& frame);

	int BuildKeepAliveRsp(std::vector<unsigned char>& bigBuffer);
	int BuildBigTestRsp(std::vector<unsigned char>& bigBuffer);
	int BuildTestImg(std::vector<unsigned char>& bigBuffer);
	int BuildResponse(int cmdToHandle, std::vector<unsigned char>& bigBuffer);

	int GetFrameCount(const std::vector<unsigned char>& bigBuffer);
	int BuildPartialFrame(int cmdToHandle, int curFrameIdx, std::vector<unsigned char>& dataBuffer, const std::vector<unsigned char>& bigBuffer);

	int ParsePoll(const std::vector<unsigned char>& packetToParse, std::vector<unsigned char>& bigBuffer);
	int CommandProc(int cmdToHandle);
	int BuildPoll(int cmdToSend, std::vector<unsigned char>& dataBuffer);
	void PDUChecksumWrite(unsigned char *dataBuffer, unsigned int length);
	unsigned int PDUChecksumCheck(const unsigned char *dataBuffer, unsigned int length);

};

#endif
