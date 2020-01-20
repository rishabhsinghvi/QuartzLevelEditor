#include "QuartzCreator.h"

#include<iostream>

int main()
{
	using namespace QuartzCreator;

	auto quartzCreatorApp = std::make_unique<QuartzCreatorApp>();

	quartzCreatorApp->init();
	quartzCreatorApp->run();
	quartzCreatorApp->shutdown();
}
