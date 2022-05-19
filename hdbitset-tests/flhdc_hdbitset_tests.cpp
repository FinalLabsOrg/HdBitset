#include "catch.hpp"
#include <memory>
#include "../HD/flhdc.h"

using namespace flhdc;

TEST_CASE("hdbitset::count_u()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("A shuffled hdbitset") {

        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.shuffle();

        THEN("count() returns the same number as the underlying bitset::count()") {
            REQUIRE(oHdbitset_10048.count_u() == static_cast<unsigned>(oHdbitset_10048.std::bitset<10048>::count()));
        }

    }

}


TEST_CASE("hdbitset::get_generator()", "[hdbitset]") {

    GIVEN("Two generators and two hdbitsets") {

        p_randengine_t pRandEngine1 = randengine_factory::factory(0, 1, 2);
        p_hdbitset10k_t pBitset1 = hdbitset10k_factory::random(pRandEngine1);

        p_randengine_t pRandEngine2 = randengine_factory::factory(0, 1, 2);
        p_hdbitset10k_t pBitset2 = hdbitset10k_factory::random(pRandEngine2);

        THEN("hdbitset::get_generator() returns the respective generators") {

            p_randengine_t pGottenRandEngine1 = pBitset1->get_randengine();
            p_randengine_t pGottenRandEngine2 = pBitset2->get_randengine();

            REQUIRE(pGottenRandEngine1 == pRandEngine1);
            REQUIRE(pGottenRandEngine2 == pRandEngine2);

        }

    }

}


TEST_CASE("hdbitset::scroll()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
    const unsigned uSize = 10048;

    GIVEN("One bitset with random values and its copy are created") {

        std::shared_ptr<hdbitset<uSize>> pHdBitset = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pHdRotatable = hdbitset_factory<uSize>::copy(pHdBitset);

        WHEN("Permutating zero times") {

            pHdRotatable->scroll(0);

            THEN("Bitset is unchanged") {
                REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

        }

        WHEN("Permutating +size() times") {

            pHdRotatable->scroll(pHdBitset->size_u());

            THEN("Bitset is unchanged") {
                REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

        }

        WHEN("Permutating -size() times") {

            pHdRotatable->scroll(-pHdBitset->size_u());

            THEN("Bitset is unchanged") {
                REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

        }

        WHEN("Permutating +1 times") {

            pHdRotatable->scroll(1);

            THEN("Bitset changes (unless very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

        }

        WHEN("Permutating -1 times") {

            pHdRotatable->scroll(-1);

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

        }

        WHEN("Permutating +size()-1 times") {

            pHdRotatable->scroll(pHdBitset->size_u() - 1);

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

            AND_WHEN("Permutating +1 times") {

                pHdRotatable->scroll(1);

                THEN("Bitset returns to initial bitset") {
                    REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
                }

            }

        }

        WHEN("Permutating +size()-5 times") {

            pHdRotatable->scroll(pHdBitset->size_u() - 5);

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

            AND_WHEN("Permutating +5 times") {

                pHdRotatable->scroll(+5);

                THEN("Bitset returns to initial bitset") {
                    REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
                }

            }

        }

        WHEN("Permutating -size()+5 times") {

            pHdRotatable->scroll(-(pHdBitset->size_u() - 5));

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

            AND_WHEN("Permutating -5 times") {

                pHdRotatable->scroll(-5);

                THEN("Bitset returns to initial bitset") {
                    REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
                }

            }

        }

        WHEN("Permutating +size()+5 times") {

            pHdRotatable->scroll(pHdBitset->size_u() + 5);

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

            AND_WHEN("Permutating -5 times") {

                pHdRotatable->scroll(-5);

                THEN("Bitset returns to initial bitset") {
                    REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
                }

            }

        }

        WHEN("Permutating -size()-5 times") {

            pHdRotatable->scroll(-pHdBitset->size_u() - 5);

            THEN("Bitset changes (unless with very low probability)") {
                CHECK_FALSE(ops<uSize>::eq(pHdBitset, pHdRotatable));
            }

            AND_WHEN("Permutating +5 times") {

                pHdRotatable->scroll(5);

                THEN("Bitset returns to initial bitset") {
                    REQUIRE(ops<uSize>::eq(pHdBitset, pHdRotatable));
                }

            }

        }


        GIVEN("One bitset with a single true value") {

            std::shared_ptr<hdbitset<uSize>> pHdBitset = hdbitset_factory<uSize>::zero(pHdRandom);
            pHdBitset->set(0, true);

            WHEN("Permutating +1") {

                pHdBitset->scroll(1);

                THEN("Bit #1 is true and bit #0 is false") {
                    REQUIRE_FALSE(pHdBitset->test(0));
                    REQUIRE(pHdBitset->test(1));
                }

                AND_WHEN("Permutating -1") {

                    pHdBitset->scroll(-1);

                    THEN("Bit #0 is true and bit #1 is false") {
                        REQUIRE(pHdBitset->test(0));
                        REQUIRE_FALSE(pHdBitset->test(1));
                    }

                }

            }

            WHEN("Permutating +size()-1") {

                pHdBitset->scroll(pHdBitset->size_u() - 1);

                THEN("Bit #size()-1 is true and bit #0 is false") {
                    REQUIRE_FALSE(pHdBitset->test(0));
                    REQUIRE(pHdBitset->test(pHdBitset->size_u() - 1));
                }

                AND_WHEN("Permutating +1") {

                    pHdBitset->scroll(+1);

                    THEN("Bit #size()-1 is false and bit #0 is true") {
                        REQUIRE(pHdBitset->test(0));
                        REQUIRE_FALSE(pHdBitset->test(pHdBitset->size_u() - 1));
                    }

                }

            }

            WHEN("Permutating -1") {

                pHdBitset->scroll(-1);

                THEN("Bit #size()-1 is true and bit #0 is false") {
                    REQUIRE_FALSE(pHdBitset->test(0));
                    REQUIRE(pHdBitset->test(pHdBitset->size_u() - 1));
                }

                AND_WHEN("Permutating -(size()-1)") {

                    pHdBitset->scroll(-(pHdBitset->size_u() - 1));

                    THEN("Bit #size()-1 is false and bit #0 is true") {
                        REQUIRE(pHdBitset->test(0));
                        REQUIRE_FALSE(pHdBitset->test(pHdBitset->size_u() - 1));
                    }

                }

            }

        }

    }

}

TEST_CASE("hdbitset::rotl()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
    const unsigned uSize = 10048;

	GIVEN("a hdbitset with only a single bit set") {

		std::shared_ptr<hdbitset<uSize>> pHdBitset = hdbitset_factory<uSize>::zero(pHdRandom);
		pHdBitset->set(0, true);

		THEN("Initially its count is 1") {

			REQUIRE(pHdBitset->count_u() == 1);

		}

		WHEN("rotl() is called") {

			pHdBitset->rotl();

			THEN("The count remains 1") {
				REQUIRE(pHdBitset->count_u() == 1);
			}

		}

	}

	GIVEN("a hdbitset with random values and its copy") {

		std::shared_ptr<hdbitset<uSize>> pShuffled = hdbitset_factory<uSize>::random(pHdRandom);
		std::shared_ptr<hdbitset<uSize>> pCopy = hdbitset_factory<uSize>::copy(pShuffled);

		WHEN("rotl() is called size() times") {

			for (unsigned u = 0; u < uSize - 1; u++) {

				pShuffled->rotl();

			}

			THEN("The copy is not equal to the rotated one (just with very low probability)") {

				CHECK_FALSE(ops<uSize>::eq(pShuffled, pCopy));

				AND_THEN("The last rotation results in a bitset equal to the original") {

					pShuffled->rotl();

					REQUIRE(ops<uSize>::eq(pShuffled, pCopy));

				}

			}

		}

	}

}

TEST_CASE("hdbitset::rotr()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
    const unsigned uSize = 10048;

	GIVEN("a hdbitset with only a single bit set") {

		std::shared_ptr<hdbitset<uSize>> pHdBitset = hdbitset_factory<uSize>::zero(pHdRandom);
		pHdBitset->set(0, true);

		THEN("Initially its count is 1") {

			REQUIRE(pHdBitset->count_u() == 1);

		}

		WHEN("rotr() is called") {

			pHdBitset->rotr();

			THEN("The count remains 1") {
				REQUIRE(pHdBitset->count_u() == 1);
			}

		}

	}

	GIVEN("a hdbitset with random values and its copy") {

		std::shared_ptr<hdbitset<uSize>> pShuffled = hdbitset_factory<uSize>::random(pHdRandom);
		std::shared_ptr<hdbitset<uSize>> pCopy = hdbitset_factory<uSize>::copy(pShuffled);

		WHEN("rotr() is called size() times") {

			for (unsigned u = 0; u < uSize - 1; u++) {

				pShuffled->rotr();

			}

			THEN("The copy is not equal to the rotated one (just with very low probability)") {

				CHECK_FALSE(ops<uSize>::eq(pShuffled, pCopy));

				AND_THEN("The last rotation results in a bitset equal to the original") {

					pShuffled->rotr();

					REQUIRE(ops<uSize>::eq(pShuffled, pCopy));

				}

			}

		}

	}

}

TEST_CASE("hdbitset::size_u()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("Constructor is called using various parameters") {

        hdbitset<5>		oHdbitset_5(pHdRandom);
        hdbitset<500>	oHdbitset_500(pHdRandom);
        hdbitset<5000>	oHdbitset_5000(pHdRandom);
        hdbitset<10048> oHdbitset_10048(pHdRandom);

        THEN("size_u() for each case returns the correct size") {

            REQUIRE(oHdbitset_5.size_u() == 5);
            REQUIRE(oHdbitset_500.size_u() == 500);
            REQUIRE(oHdbitset_5000.size_u() == 5000);
            REQUIRE(oHdbitset_10048.size_u() == 10048);

        }

    }

}

TEST_CASE("hdbitset::shuffle()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("An all-zero hdbitset that is shuffled") {

        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.reset();
        oHdbitset_10048.shuffle();

        THEN("the new bitset is not uniform()") {
            REQUIRE_FALSE(oHdbitset_10048.uniform());
        }

        THEN("Number of set bits is usually within 10% range of midpoint") {
            double dRatio = (double)oHdbitset_10048.std::bitset<10048>::count() / (double)10048;
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);
        }

    }

    GIVEN("An all-one hdbitset that is shuffled") {

        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.set();
        oHdbitset_10048.shuffle();

        THEN("the new bitset is not uniform()") {
            REQUIRE_FALSE(oHdbitset_10048.uniform());
        }

        THEN("Number of set bits is usually within 10% range of midpoint") {
            double dRatio = (double)oHdbitset_10048.std::bitset<10048>::count() / (double)10048;
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);
        }

    }

}

TEST_CASE("hdbitset::shuffle(uShuffledWidth)", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("An all-zero hdbitset that is shuffled using uShuffledWidth") {

        unsigned uShuffledWidth = 5000;
        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.reset();
        oHdbitset_10048.shuffle(uShuffledWidth);

        THEN("the new bitset is not uniform()") {
            REQUIRE_FALSE(oHdbitset_10048.uniform());
        }

        THEN("Number of set bits is usually within 10% range of the uShuffledWidth midpoint") {
            double dRatio = (double)oHdbitset_10048.std::bitset<10048>::count() / (double)uShuffledWidth;
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);
        }

    }

    GIVEN("An all-one hdbitset that is shuffled") {

        unsigned uShuffledWidth = 5000;
        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.set();
        oHdbitset_10048.shuffle(uShuffledWidth);

        THEN("the new bitset is not uniform()") {
            REQUIRE_FALSE(oHdbitset_10048.uniform());
        }

        THEN("Number of set bits is usually within 10% range of midpoint") {
            double dRatio = (double)oHdbitset_10048.std::bitset<10048>::count() / (double)uShuffledWidth;
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);
        }

    }

}

TEST_CASE("hdbitset::uniform()", "[hdbitset]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("Constructor is called and bitset is reset") {

        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.reset();

        THEN("all() is false, none() is true, and uniform() is true") {

            REQUIRE_FALSE(oHdbitset_10048.all());
            REQUIRE(oHdbitset_10048.none());
            REQUIRE(oHdbitset_10048.uniform());

        }

    }

    GIVEN("Constructor is called and bitset is set") {

        hdbitset<10048> oHdbitset_10048(pHdRandom);
        oHdbitset_10048.set();

        THEN("all() is true, none() is false, and uniform() is true") {

            REQUIRE(oHdbitset_10048.all());
            REQUIRE_FALSE(oHdbitset_10048.none());
            REQUIRE(oHdbitset_10048.uniform());

        }

    }

}


