/*
 * CrossRoadStartTest.hpp
 *
 *  Created on: 26.03.2021
 *      Author: Gerd
 */

#ifndef INCLUDE_STARTTEST_HPP_
#define INCLUDE_STARTTEST_HPP_

#include "CrossRoadBaseTest.hpp"

class StartTest : public CrossRoadBaseTest{
public:
	using this_type = StartTest;

	//==============================
	// Tests
	//==============================
	void initIgnoreEvent(); // used by different tests
	void initStopTimer(); // used by different tests

	void testConstuctor();
	virtual void initConstructor();

	void testOFF_off_AfterCtor(); // ignore event
	void testOFF_off_AfterFlashing(); // ignore event
	void testOFFMinDuration_off(); // ignore event

	void testOFF_flash();
	virtual void initOFF_flash();

	void testOFFMinDuration_flash_deferred();
	virtual void initOFFMinDuration_flash_deferred();

	void testFlashing_flash(); // ignore event
	void testFlashingMinDuration_flash(); // ignore event

	void testFlashing_off();
	virtual void initFlashing_off();

	void testFlashingMinDuration_off_deferred();
	virtual void initFlashingMinDuration_off_deferred();

	void testFlashing_on();
	virtual void initFlashing_on();
	void testFlashingMinDuration_on_deferred();
	virtual void initFlashingMinDuration_on_deferred();
public:
	template<class DerivedTest = this_type>
	static cute::suite make_suite(){
		cute::suite s { };
		s.push_back(CUTE_SMEMFUN(DerivedTest, testConstuctor));

		s.push_back(CUTE_SMEMFUN(DerivedTest, testOFF_off_AfterCtor));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testOFF_off_AfterFlashing));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testOFFMinDuration_off));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testOFF_flash));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testOFFMinDuration_flash_deferred));

		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashing_flash));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashingMinDuration_flash));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashing_off));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashingMinDuration_off_deferred));

		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashing_on));
		s.push_back(CUTE_SMEMFUN(DerivedTest, testFlashingMinDuration_on_deferred));

		return s;
	}
};
//=====================================
// start Tests
//=====================================
inline
void StartTest::initIgnoreEvent(){
	// ignore event
}
inline
void StartTest::initStopTimer(){
	timer.stopTimer();
}
//--------------------------------
inline
void StartTest::testConstuctor(){
	rm.beginInit();
	initConstructor();
	rm.endInit();

	rm.beginTest();
	getSUT();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initConstructor(){
	a1.off(); a2.off(); a3.off();
}
//--------------------------------
inline
void StartTest::testOFF_off_AfterCtor(){
	rm.beginInit();
	initIgnoreEvent();
	rm.endInit();

	auto &sut = getSUT();

	rm.beginTest();
	sut.off();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
//--------------------------------
inline
void StartTest::testOFF_off_AfterFlashing(){
	rm.beginInit();
	initIgnoreEvent();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	sut.off();		// off -> deferred
	timer.tick();	// Flashing -> OffMinDuration
	timer.tick();	// Off


	rm.beginTest();
	sut.off();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
//--------------------------------
inline
void StartTest::testOFFMinDuration_off(){
	rm.beginInit();
	initStopTimer();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	timer.tick();	// Flashing
	sut.off();		// OffMinDuration
	sut.off();		// ignore event

	rm.beginTest();
	timer.tick();	// Off
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
//--------------------------------
inline
void StartTest::testOFF_flash(){
	rm.beginInit();
	initOFF_flash();
	rm.endInit();

	auto &sut = getSUT();
	rm.beginTest();
	sut.flash();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initOFF_flash(){
	a1.flash(); a2.flash(); a3.flash();
	timer.setIntervalDuration(IntervalDuration(SUT::FlashingMinDuration));
	timer.startTimer();

}
//--------------------------------
inline
void StartTest::testFlashing_flash(){
	rm.beginInit();
	initIgnoreEvent();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	timer.tick();	// Flashing

	rm.beginTest();
	sut.flash();	// ignore event
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
//--------------------------------
inline
void StartTest::testFlashingMinDuration_flash(){
	rm.beginInit();
	initStopTimer();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	sut.flash();	// ignore event

	rm.beginTest();
	timer.tick();	// Flashing
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
//--------------------------------
inline
void StartTest::testFlashing_off(){
	rm.beginInit();
	initFlashing_off();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	timer.tick();	// Flashing

	rm.beginTest();
	sut.off();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initFlashing_off(){
	a1.off(); a2.off(); a3.off();
	timer.setIntervalDuration(IntervalDuration(SUT::OffMinDuration));
	timer.startTimer();

}
//--------------------------------
inline
void StartTest::testFlashingMinDuration_off_deferred(){
	rm.beginInit();
	initFlashingMinDuration_off_deferred();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash(); // FlashingMinDuration

	rm.beginTest();
	sut.off(); // deferred
	timer.tick(); // Flashing->off->OffMinDuration
	rm.endTest();

//	ASSERT_EQUAL(timer.getIntervalDuration().count(), SUT::OffMinDuration);
	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initFlashingMinDuration_off_deferred(){
	timer.stopTimer();
	a1.off(); a2.off(); a3.off();
	timer.setIntervalDuration(IntervalDuration(SUT::OffMinDuration));
	timer.startTimer();
}
//--------------------------------
inline
void StartTest::testOFFMinDuration_flash_deferred(){
	rm.beginInit();
	initOFFMinDuration_flash_deferred();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();	// FlashingMinDuration
	timer.tick();	// Flashing
	sut.off();		// OffMinDuration

	sut.flash();	// -> deferred

	rm.beginTest();
	timer.tick();	// Off->flash->FlashingMinDuration
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initOFFMinDuration_flash_deferred(){
	timer.stopTimer();
	a1.flash(); a2.flash(); a3.flash();
	timer.setIntervalDuration(IntervalDuration(SUT::FlashingMinDuration));
	timer.startTimer();
}
//--------------------------------
void StartTest::testFlashing_on(){
	rm.beginInit();
	initFlashing_on();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash();
	timer.tick(); // Flashing

	rm.beginTest();
	sut.on();
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initFlashing_on(){
	a1.switchOver(); a2.switchOver(); // Yellow
	timer.setIntervalDuration(IntervalDuration(SUT::MinorFlashing));
	timer.startTimer();
}
//--------------------------------
inline
void StartTest::testFlashingMinDuration_on_deferred(){
	rm.beginInit();
	initFlashingMinDuration_on_deferred();
	rm.endInit();

	auto &sut = getSUT();
	sut.flash(); // FlashingMinDuration

	rm.beginTest();
	sut.on(); // deferred
	timer.tick(); // Flashing
	rm.endTest();

	ASSERT_EQUAL(rm.getExpected(), rm.getResult());
}
inline
void StartTest::initFlashingMinDuration_on_deferred(){
	timer.stopTimer();
		a1.switchOver(); a2.switchOver(); // Yellow
		timer.setIntervalDuration(IntervalDuration(SUT::MinorFlashing));
		timer.startTimer();
}

#endif /* INCLUDE_STARTTEST_HPP_ */
