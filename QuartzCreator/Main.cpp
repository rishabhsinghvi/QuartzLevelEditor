#include "QuartzCreator.h"

#include<iostream>

int main()
{
	using namespace QuartzCreator;

	auto quartzCreatorApp = std::make_unique<QuartzCreatorApp>();

	quartzCreatorApp->Init();
	quartzCreatorApp->run();
	quartzCreatorApp->shutdown();
}
