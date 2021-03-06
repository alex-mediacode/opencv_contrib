/*
 *  By downloading, copying, installing or using the software you agree to this license.
 *  If you do not agree to this license, do not download, install,
 *  copy or use the software.
 *
 *
 *  License Agreement
 *  For Open Source Computer Vision Library
 *  (3 - clause BSD License)
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met :
 *
 *  *Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and / or other materials provided with the distribution.
 *
 *  * Neither the names of the copyright holders nor the names of the contributors
 *  may be used to endorse or promote products derived from this software
 *  without specific prior written permission.
 *
 *  This software is provided by the copyright holders and contributors "as is" and
 *  any express or implied warranties, including, but not limited to, the implied
 *  warranties of merchantability and fitness for a particular purpose are disclaimed.
 *  In no event shall copyright holders or contributors be liable for any direct,
 *  indirect, incidental, special, exemplary, or consequential damages
 *  (including, but not limited to, procurement of substitute goods or services;
 *  loss of use, data, or profits; or business interruption) however caused
 *  and on any theory of liability, whether in contract, strict liability,
 *  or tort(including negligence or otherwise) arising in any way out of
 *  the use of this software, even if advised of the possibility of such damage.
 */

#include "perf_precomp.hpp"

namespace cvtest
{

using std::tr1::tuple;
using std::tr1::get;
using namespace perf;
using namespace testing;
using namespace cv;
using namespace cv::ximgproc;

typedef tuple<Size, MatType, int> L0SmoothTestParam;
typedef TestBaseWithParam<L0SmoothTestParam> L0SmoothTest;

PERF_TEST_P(L0SmoothTest, perf,
    Combine(
    SZ_TYPICAL,
    Values(CV_8U, CV_16U, CV_32F, CV_64F),
    Values(1, 3))
)
{
    L0SmoothTestParam params = GetParam();
    Size sz         = get<0>(params);
    int depth       = get<1>(params);
    int srcCn        = get<2>(params);

    Mat src(sz, CV_MAKE_TYPE(depth, srcCn));
    Mat dst(sz, src.type());

    cv::setNumThreads(cv::getNumberOfCPUs());
    declare.in(src, WARMUP_RNG).out(dst).tbb_threads(cv::getNumberOfCPUs());

    RNG rnd(sz.height + depth + srcCn);
    double lambda = rnd.uniform(0.01, 0.05);
    double kappa = rnd.uniform(1.0, 3.0);

    TEST_CYCLE_N(1)
    {
        l0Smooth(src, dst, lambda, kappa);
    }

    SANITY_CHECK_NOTHING();
}
}
