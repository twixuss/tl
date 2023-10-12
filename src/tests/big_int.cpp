#include <tl/big_int.h>

using namespace tl;

void big_int_test() {
	assert(1_ib == 1_ib);
	assert(1_ib + 2_ib == 3_ib);
	assert(1_ib - 2_ib == -1_ib);
	assert(1_ib << 4 == 16_ib);
	assert(3_ib * 4_ib == 12_ib);
	assert((0xfa_ib & 0xf_ib) == 0xa_ib);
	assert(( 5421_ib &  6748_ib) == 4108_ib);
	assert((-5421_ib & -6748_ib) == -8064_ib);
	assert((-5421_ib &  6748_ib) == 2640_ib);
	assert(( 5421_ib & -6748_ib) == 1316_ib);
	assert(( 5421_ib |  6748_ib) == 8061_ib);
	assert((-5421_ib | -6748_ib) == -4105_ib);
	assert((-5421_ib |  6748_ib) == -1313_ib);
	assert(( 5421_ib | -6748_ib) == -2643_ib);
	assert(0x8000000000000000_ib * 2_ib == make_big_int(1, 0));


	// fffffffffff0f0
	// ffffffffffff0f

	BigInt a = 0x04aa8a77af273627_ib * 0x0c1977bf353eaa48_ib;
	BigInt b = 0x62049fd081f10577_ib * 0x47566041d43542a3_ib;
	assert(a == make_big_int(0x387553E4795627, 0xE6667DC77D6F20F8));
	assert(b == make_big_int(0x1B505ABB74707CF8, 0x932505E68D8228C5));
	//BigInt a = 3_ib;
	//BigInt b = 4_ib;
	//auto c = a * b;

	// REMEMBER:
	// LEAST SIGNIFICANT PART GOES FIRST!
	{
		// &
		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full})}));



		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full      })}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full      })}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full, 1ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full, 1ull})}));



		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full,     })}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} &
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full, 1ull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full      })}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} &
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x00000000000000f0ull, 0x000000f00000000full, 1ull})}));
	}
	{
		// |
		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull})}));



		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull, 1ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull, 1ull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull      })}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull,     })}));



		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull, 1ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} |
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull,     })}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true , .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull, 1ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} |
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f0000ffull, 0x000f00f0f0f000ffull,     })}));
	}
	{
		// ^
		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 1ull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 1ull})}));



		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 0xfffffffffffffffeull})}));

		assert(((BigInt{.msb=false, .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true , .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 1ull})}));



		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 1ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} ^
				 BigInt{.msb=false, .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=true,  .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 0xfffffffffffffffeull})}));



		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full})} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full, 1ull})} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull      })}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 0xfffffffffffffffeull})}));

		assert(((BigInt{.msb=true,  .parts=make_list({0x000000f0000000ffull, 0x000f00f0f000000full      })} ^
				 BigInt{.msb=true,  .parts=make_list({0x000000000f0000f0ull, 0x000000f000f000ffull, 1ull})}) ==
				BigInt{.msb=false, .parts=make_list({0x000000f00f00000full, 0x000f0000f0f000f0ull, 0xfffffffffffffffeull})}));
	}
}