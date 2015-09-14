#include <stdio.h>
#include <string.h>
#include "commandHandle.h"
#include <iostream>
#include <tr1/memory>

//#define CMD_DEBUG

#define MAX_PKT_SIZE 10000

void COMMAND_HANDLER::UnitTests(void)
{
	unsigned char testPDUGoodData[] = {HEADER_BYTE_1, HEADER_BYTE_2, 13, 0, UNIT_ID, CMD_KEEPALIVE, 0x00, 0x00, 0x01, 0x02, 0x03, 0xAD, 0x00};
	unsigned char testPDUBad1Data[] = {HEADER_BYTE_1, 0xFF, 11, 0, UNIT_ID, CMD_KEEPALIVE, 0x00, 0x00, 0x01, 0x02, 0x03, 0x00, 0x00};
	unsigned char testPDUBad2Data[] = {HEADER_BYTE_1, HEADER_BYTE_2, 1, 0, UNIT_ID, CMD_KEEPALIVE, 0x00, 0x00, 0x01, 0x02, 0x03, 0x00, 0x00};
	unsigned char testPDUBad3Data[] = {HEADER_BYTE_1, HEADER_BYTE_2, 13, 0, UNIT_ID, CMD_KEEPALIVE, 0x00, 0x00, 0x01, 0x02, 0x03, 0xAC, 0x00};
	unsigned char testPDUBad4Data[] = {HEADER_BYTE_1, HEADER_BYTE_2, 12, 0, UNIT_ID, CMD_KEEPALIVE, 0x00, 0x00, 0x01, 0x02, 0x03, 0xAD, 0x00};
	std::vector<unsigned char> testPoll;
	std::vector<unsigned char> testResponse;
	std::vector<unsigned char> testBigBuffer;
	std::vector<unsigned char> testBigData;
	std::vector<unsigned char> testPDUGood(testPDUGoodData, testPDUGoodData + sizeof(testPDUGoodData) / (sizeof(testPDUGoodData[0])));
	std::vector<unsigned char> testPDUBad1(testPDUBad1Data, testPDUBad1Data + sizeof(testPDUBad1Data) / (sizeof(testPDUBad1Data[0])));
	std::vector<unsigned char> testPDUBad2(testPDUBad2Data, testPDUBad2Data + sizeof(testPDUBad2Data) / (sizeof(testPDUBad2Data[0])));
	std::vector<unsigned char> testPDUBad3(testPDUBad3Data, testPDUBad3Data + sizeof(testPDUBad3Data) / (sizeof(testPDUBad3Data[0])));
	std::vector<unsigned char> testPDUBad4(testPDUBad4Data, testPDUBad4Data + sizeof(testPDUBad4Data) / (sizeof(testPDUBad4Data[0])));
	unsigned int i, frameCnt;

	printf("\r\nunitTests\r\n");

	//Helper functions
	CommandProc(CMD_KEEPALIVE);
	CommandProc(0xFF);
	ParsePoll(testPDUGood, testBigData);
	ParsePoll(testPDUBad1, testBigData);
	ParsePoll(testPDUBad2, testBigData);
	ParsePoll(testPDUBad3, testBigData);
	ParsePoll(testPDUBad4, testBigData);

	BuildResponse(CMD_KEEPALIVE, testBigBuffer);
	BuildPartialFrame(CMD_KEEPALIVE, 0, testResponse, testBigBuffer);
	ParsePoll(testResponse, testBigBuffer);

	BuildResponse(CMD_TESTBIGPKT, testBigBuffer);

	frameCnt = GetFrameCount(testBigBuffer);

	printf("\r\nTest Frame Cnt:%d", frameCnt);

	BuildPartialFrame(CMD_TESTBIGPKT, 0, testResponse, testBigBuffer);
	ParsePoll(testResponse, testBigData);
	BuildPartialFrame(CMD_TESTBIGPKT, 1, testResponse, testBigBuffer);
	ParsePoll(testResponse, testBigData);

	printf("\r\nTestBigDataResult:");
	for ( i = 0; i < testBigData.size(); i++ )
	{
		printf("[%02X]", testBigData[i]);
		if ( testBigBuffer[i] != testBigData[i] )
		{
			printf("\r\nBIG TEST FAILED\r\n");
			break;
		}
	}

	BuildPartialFrame(CMD_TESTBIGPKT, 3, testResponse, testBigBuffer); //should return error (big pkt is only 2000 bytes)

	BuildPoll(CMD_KEEPALIVE,testPoll);
	ParsePoll(testPoll, testBigData);

	testBigBuffer.clear();
	BuildTestImg(testBigBuffer);
	frameCnt = GetFrameCount(testBigBuffer);
	printf("\r\nTest Frame Cnt:%d\r\n", frameCnt);

	for ( i = 0; i < frameCnt; i++ )
	{
		BuildPartialFrame(CMD_TESTIMG, i, testResponse, testBigBuffer);
		ParsePoll(testResponse, testBigData);
	}
#ifdef CMD_DEBUG
	cv::Mat testMat = cv::imdecode(testBigData, CV_LOAD_IMAGE_COLOR);
	cv::imshow("test", testMat);
	cv::waitKey(0);
#endif

	printf("\r\n");
}

//Returns 0 if successful
int COMMAND_HANDLER::EchoCmdHandle(void)
{
#ifdef CMD_DEBUG
	printf("\r\nRun Echo Handle\r\n");
#endif
	return 0;
}

//Returns 0 if successful
int COMMAND_HANDLER::CommandProc(int cmdToHandle)
{
	int ret = -1;

#ifdef CMD_DEBUG
	printf("\r\nCommand Handler %d\r\n", cmdToHandle);
#endif

	switch ( cmdToHandle )
	{
		case CMD_KEEPALIVE:
			ret = EchoCmdHandle();
			break;
		case CMD_TESTBIGPKT:
		case CMD_TESTIMG:
			ret = 0;
			break;
		case CMD_STREAMCAM:
			ret = 0;
			break;
		default:
			printf("\r\nCommand not recognized\r\n");
			break;
	}
	
	return (ret);
}

//Returns -1 if error, 0 if the poll was successful but we are expecting more frames, 1 if we've received all frames and can process data
int COMMAND_HANDLER::ParsePoll(const std::vector<unsigned char>& packetToParse, std::vector<unsigned char>& bigBuffer)
{
	int ret = -1;
	unsigned int idx = 0;
	unsigned int PDULength;
	unsigned char unitID;
	unsigned char pollType;
	unsigned int curFrameIdx, endFrameIdx;
	unsigned int i;
	unsigned int csum, expectedCsum;
	std::vector<unsigned char>::const_iterator it;
#ifdef CMD_DEBUG
	printf("\r\nParse Poll\r\n");
#endif
	//The vector size should at LEAST be enough to get the PDU Length
	if ( packetToParse.size() < 4 )
	{
		printf("\r\nVector too small!\r\n");
		return -1;
	}

	it = packetToParse.begin();

	//Header bytes
	if ( *it++ != HEADER_BYTE_1 )
	{
		printf("\r\nBad Header\r\n");
		return -1;
	}
	if ( *it++ != HEADER_BYTE_2 )
	{
		printf("\r\nBad Header\r\n");
		return -1;
	}
	
	PDULength = *it++;
	PDULength |= *it++ << 8;
#ifdef CMD_DEBUG
	printf("\r\nPDU Length:%d\r\n", PDULength);
#endif
	//The PDU length includes all of the overhead so remove it in the calculation
	if ( PDULength < 8 )
	{
		printf("\r\nPDU Length too small\r\n");
		return -1;
	}
	else if ( PDULength > (MAX_PKT_SIZE+12) ) //Count ALL of the header bytes (12)
	{
		printf("\r\nPDU Length too long\r\n");
		return -1;
	}
	else if (PDULength != packetToParse.size()) //Make sure the PDU size actually matches the vector passed in it
	{
		printf("\r\nPDU size mismatch");
		return -1;
	}

	//Unit ID
	unitID = *it++;
	if ( unitID != UNIT_ID )
	{
		printf("\r\nBad UnitID %d", unitID);
		return -1;
	}
#ifdef CMD_DEBUG
	printf("\r\nUnit ID:%d\r\n", unitID);
#endif
	//Poll Type
	pollType = *it++;
#ifdef CMD_DEBUG
	printf("\r\nPoll Type:%d\r\n", pollType);
#endif
	//Frame Info
	curFrameIdx = *it++;
	curFrameIdx |= *it++ << 8;
	endFrameIdx = *it++;
	endFrameIdx |= *it++ << 8;
#ifdef CMD_DEBUG
	printf("\r\nFrame Info:%d.%d\r\n", curFrameIdx, endFrameIdx);
#endif
	//Make sure we clear the big buffer!
	if ( curFrameIdx == 0 )
		bigBuffer.clear();

	//Poll Data
#ifdef CMD_DEBUG
	printf("\r\nPoll Data:");
#endif
	//Subtract everythign BEFORE data
	for ( i = 0; i < (PDULength - 12); i++ )
	{
#ifdef CMD_DEBUG
		printf("[%02X]", *it);
#endif
		bigBuffer.push_back(*it++);
	}
#ifdef CMD_DEBUG
	printf("\r\n");
#endif

	//Checksum
	csum = *it++;
	csum |= *it++ << 8;

	expectedCsum = PDUChecksumCheck(packetToParse.data(), PDULength-2);

	if ( csum != expectedCsum )
	{
		printf("\r\nBad csum found:%04X expected: %04X\r\n", csum, expectedCsum);
		return -1;
	}
#ifdef CMD_DEBUG
	printf("\r\nValid PDU\r\n");
#endif

	return (curFrameIdx == endFrameIdx);
}

int COMMAND_HANDLER::BuildPoll(int cmdToSend, std::vector<unsigned char>& dataBuffer)
{
	int ret = 0;
	unsigned int idx, length;
	unsigned int csum;
#ifdef CMD_DEBUG
	printf("\r\nBuild Poll %d\r\n", cmdToSend);
#endif

	//Make sure we clear the vector beforehand
	dataBuffer.clear();

	//Format packet (length will be filled out last before checksum)
	dataBuffer.push_back(HEADER_BYTE_1);
	dataBuffer.push_back(HEADER_BYTE_2);

	//allocate space for the length bytes NOW - this saves a little bit of processing time since it won't reallocate
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);

	//Unit ID
	dataBuffer.push_back(UNIT_ID);

	//Poll Type
	dataBuffer.push_back(cmdToSend);

	//Frame Info - poll requests will never have more than one frame
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);

	switch ( cmdToSend )
	{
		case CMD_KEEPALIVE:
		case CMD_TESTBIGPKT:
		case CMD_TESTIMG:
			break;
		default:
			printf("\r\nCommand not recognized\r\n");
			return -1;
			break;
	}

	//Write the length to the vector - first we create space for the checksum
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);
	length = dataBuffer.size();
	dataBuffer[2] = length & 0xFF;
	dataBuffer[3] = (length >> 8) & 0xFF;
	PDUChecksumWrite(dataBuffer.data(), length-2); //Subtract 2 since we don't include the checksum itself

#ifdef CMD_DEBUG
	printf("\r\npollPDU:");
	{
		std::vector<unsigned char>::iterator it;
		for ( it = dataBuffer.begin(); it != dataBuffer.end(); it++ )
		{
			//printf("[%02X]", *it); //JJV DEBUG
		}
	}

	printf("\r\n");
#endif

	return (ret);
}

//Returns the number of frames that would be required for the data portion being sent (we allow 1200 bytes of data at the most)
int COMMAND_HANDLER::GetFrameCount(const std::vector<unsigned char>& bigBuffer)
{
	return ((bigBuffer.size() / MAX_PKT_SIZE) + 1);
}

//Returns success
int COMMAND_HANDLER::BuildPartialFrame(int cmdToHandle, int curFrameIdx, std::vector<unsigned char>& dataBuffer, const std::vector<unsigned char>& bigBuffer)
{
	std::vector<unsigned char>::const_iterator it;
	std::vector<unsigned char>::iterator buffIt;
	int count;
	int endFrameIdx = (bigBuffer.size() / MAX_PKT_SIZE);
	unsigned int length;

	count = 0;

#ifdef CMD_DEBUG
	printf("\r\nBuild Partial Frame\r\n");
#endif

	//Make sure we clear the vector beforehand
	dataBuffer.clear();

	//Format packet (length will be filled out last before checksum)
	dataBuffer.push_back(HEADER_BYTE_1);
	dataBuffer.push_back(HEADER_BYTE_2);

	//allocate space for the length bytes NOW - this saves a little bit of processing time since it won't reallocate
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);

	//Unit ID
	dataBuffer.push_back(UNIT_ID);

	//Poll Type
	dataBuffer.push_back(cmdToHandle | 0x80);

	//Frame Info
	dataBuffer.push_back(curFrameIdx & 0xFF);
	dataBuffer.push_back((curFrameIdx >> 8) & 0xFF);
	dataBuffer.push_back(endFrameIdx & 0xFF);
	dataBuffer.push_back((endFrameIdx >> 8) & 0xFF);

	if ( curFrameIdx > endFrameIdx )
	{
		printf("\r\nFrame Count Error!\r\n");
		return -1;
	}

	it = bigBuffer.begin();

	//If we have to send multiple frames, make sure we start in the correct position of the big data buffer
	if ( endFrameIdx > 0 )
	{
		count = 0;
		while (count++ < curFrameIdx*MAX_PKT_SIZE)
		{
			it++;
		}
	}

	//Write the big buffer data to our packet to be sent
	if ( curFrameIdx < endFrameIdx )
	{
		for ( count = 0; count < MAX_PKT_SIZE; count++, it++ )
		{
			dataBuffer.push_back(*it);
		}
	}
	else
	{
		while ( it != bigBuffer.end() )
		{
			dataBuffer.push_back(*it);
			it++;
		}
	}


	//Write the length to the vector - first we create space for the checksum
	dataBuffer.push_back(0);
	dataBuffer.push_back(0);
	length = dataBuffer.size();
	dataBuffer[2] = length & 0xFF;
	dataBuffer[3] = (length >> 8) & 0xFF;
	PDUChecksumWrite(dataBuffer.data(), length-2); //Subtract 2 since we don't include the checksum itself
#ifdef CMD_DEBUG
	printf("\r\nresponsePDUPartial:");

	for ( buffIt = dataBuffer.begin(); buffIt != dataBuffer.end(); buffIt++ )
	{
		printf("[%02X]", *buffIt);
	}

	printf("\r\n");
#endif

	return 0;
}


//Returns success
int COMMAND_HANDLER::BuildKeepAliveRsp(std::vector<unsigned char>& bigBuffer)
{
	bigBuffer.push_back(0x01);
	bigBuffer.push_back(0x02);
	bigBuffer.push_back(0x03);

	return 0;
}

int COMMAND_HANDLER::BuildBigTestRsp(std::vector<unsigned char>& bigBuffer)
{
	int i;

	for ( i = 0; i < 2000; i++ )
		bigBuffer.push_back(i % 255);

	return 0;
}

int COMMAND_HANDLER::BuildTestImg(std::vector<unsigned char>& bigBuffer)
{
	cv::Mat testMat = cv::imread("test.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat testMatConverted;
	unsigned int baseIndex = 0, i, j;
	unsigned int imgSize;

	testMat.convertTo(testMatConverted, CV_8U);

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(95);

	//imgSize = testMatConverted.total()*testMatConverted.elemSize();
	//bigBuffer.resize(imgSize);

	cv::imencode(".jpeg", testMatConverted, bigBuffer, compression_params);
#ifdef CMD_DEBUG
	printf("newBuff size:%lu", bigBuffer.size());
#endif

/*
	for ( i = 0; i < imgSize; i++ )
	{
		bigBuffer.push_back(testMatConverted.data[i]);
	}
*/

	return 0;
}

int COMMAND_HANDLER::BuildFrameResp(std::vector<unsigned char>& bigBuffer, cv::Mat& frame)
{
	int ret = 0;
	cv::Mat MatConverted;

	//Make sure we clear the vector beforehand
	bigBuffer.clear();
	frame.convertTo(MatConverted, CV_8U);

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(25);

	cv::imencode(".jpeg", MatConverted, bigBuffer, compression_params);

	return (ret);
}

//Returns success - populate the big buffer we want to send
int COMMAND_HANDLER::BuildResponse(int cmdToHandle, std::vector<unsigned char>& bigBuffer)
{
	int ret = 0;
#ifdef CMD_DEBUG
	printf("\r\nBuild Response %d\r\n", cmdToHandle);
#endif

	//Make sure we clear the vector beforehand
	bigBuffer.clear();

	switch ( cmdToHandle )
	{
		case CMD_KEEPALIVE:
			ret = BuildKeepAliveRsp(bigBuffer);
			break;
		case CMD_TESTBIGPKT:
			ret = BuildBigTestRsp(bigBuffer);
			break;
		case CMD_TESTIMG:
			ret = BuildTestImg(bigBuffer);
			break;
		default:
			printf("\r\nCommand not recognized\r\n");
			return -1;
			break;
	}
#ifdef CMD_DEBUG
	printf("\r\nresponseData:");
	{
		std::vector<unsigned char>::iterator it;
		for ( it = bigBuffer.begin(); it != bigBuffer.end(); it++ )
		{
			printf("[%02X]", *it);
		}
	}

	printf("\r\n");
#endif
	return (ret);
}

unsigned int COMMAND_HANDLER::PDUChecksumCheck(const unsigned char *dataBuffer, unsigned int length)
{
	unsigned int csum = 0;
	unsigned int i;

	if ( dataBuffer == NULL )
	{
		printf("\r\ndata NULL?\r\n"); //JJV DEBUG
		return -1;
	}

	csum = 0;
	for ( i = 0; i < length; i++ )
	{
		csum += *dataBuffer++;
	}

	csum &= 0xFFFF;

	return csum;
}

void COMMAND_HANDLER::PDUChecksumWrite(unsigned char *dataBuffer, unsigned int length)
{
	unsigned int csum = 0;
	unsigned int i;

	if ( dataBuffer == NULL )
	{
		printf("\r\ndata NULL?\r\n"); //JJV DEBUG
		return;
	}

	csum = 0;
	for ( i = 0; i < length; i++ )
	{
		csum += *dataBuffer++;
	}

	csum &= 0xFFFF;

	*dataBuffer++ = (csum & 0xFF);
	*dataBuffer++ = ((csum >> 8) & 0xFF);
}









