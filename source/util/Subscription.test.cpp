//
// Created by Ruurd Adema on 15/02/2024.
// Copyright (c) 2024 Sound on Digital. All rights reserved.
//

#include <catch2/catch.hpp>

#include "rdk/util/Subscription.h"

TEST_CASE ("Count destruction callbacks", "[Subscription]")
{
	int count = 0;

	{
		rdk::Subscription subscription([&count]
		{
			count++;
		});

		REQUIRE(count == 0);
	}
	REQUIRE(count == 1);
}
