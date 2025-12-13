CC=g++
CFLAGS=-Iheader -Wall -std=c++17
OPENCV_C=$(shell pkg-config --cflags opencv4)
OPENCV_L=$(shell pkg-config --libs opencv4)

videoreceiver_test:	MReceiver.o MCommunicator.o Logger.o FrameReceiver.o VFrameExtractor.o CSemaphore.o SharedMem.o VideoReceiver_test.o
	$(CC) $^ -o $@

videowindow_test: Logger.o CSemaphore.o SharedMem.o VideoWindow_test.o
	$(CC) $^ -o $@ $(OPENCV_L)

framereceiver_test: MReceiver.o MCommunicator.o Logger.o Framereceiver_test.o FrameReceiver.o VFrameExtractor.o
	$(CC) $^ -o $@ $(OPENCV_L)

framesender_test: MSender.o MCommunicator.o Logger.o Framesender_test.o FrameSender.o VFrameBuilder.o
	$(CC) $^ -o $@ $(OPENCV_L)

mreceiver_test: MReceiver.o MCommunicator.o Logger.o Mreceiver_test.o
	$(CC) $^ -o $@

msender_test: MSender.o MCommunicator.o Logger.o Msender_test.o
	$(CC) $^ -o $@

VideoReceiver_test.o: test/VideoReceiver_test.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%_test.o: test/%_test.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(OPENCV_C)

%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm -f *.o msender_test mreceiver_test framesender_test framereceiver_test videoreceiver_test videowindow_test
