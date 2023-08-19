#include "pch.h"

extern "C" int Test(void);

TEST_CLASS(SimpleMQTTPublisherTest)
{
public:
		
	TEST_METHOD(TestMethod1)
	{
		Assert::AreEqual(42, Test());
	}
};
