/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * This file was autogenerated from GrEllipseEffect.fp; do not modify.
 */
#include "GrEllipseEffect.h"
#if SK_SUPPORT_GPU
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLEllipseEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLEllipseEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrEllipseEffect& _outer = args.fFp.cast<GrEllipseEffect>();
        (void)_outer;
        prevRadii = half2(-1.0);
        useScale = sk_Caps.floatPrecisionVaries;
        fEllipseVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHighFloat4_GrSLType,
                                                       kDefault_GrSLPrecision, "ellipse");
        if (useScale) {
            fScaleVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf2_GrSLType,
                                                         kDefault_GrSLPrecision, "scale");
        }
        fragBuilder->codeAppendf(
                "half2 prevCenter;\nhalf2 prevRadii = half2(%f, %f);\nbool useScale = %s;\nhalf2 d "
                "= half2(sk_FragCoord.xy - %s.xy);\n@if (useScale) {\n    d *= %s.y;\n}\nhalf2 Z = "
                "d * half2(%s.zw);\nhalf implicit = dot(Z, d) - 1.0;\nhalf grad_dot = 4.0 * dot(Z, "
                "Z);\ngrad_dot = half(max(highfloat(grad_dot), 0.0001));\nhalf approx_dist = "
                "highfloat(implicit) * inversesqrt(highfloat(grad_dot));\n@if (useScale) {\n    "
                "approx_dist *= %s.x;\n}\nhalf alpha;\n@switch (%d) {\n    case 0:\n        alpha "
                "= half(highfloat(approx_dist) > 0.0 ? 0.0 : 1.0);\n        break;\n    case 1:\n  "
                "      alpha = half(clamp(0.5 - highfloat(approx_dist), 0.0, 1.0));\n        "
                "break;\n    case 2:\n        alpha = half(highfloat(approx_dist) > 0.0 ? 1.0 : "
                "0.0);\n        break;\n    case 3:\n        alpha = half(clamp(0.5 + "
                "highfloat(approx_dist), 0.0, 1.0));\n        break;\n    default:\n        "
                "discard;\n}\n%s = %s * alpha;\n",
                prevRadii.fX, prevRadii.fY, (useScale ? "true" : "false"),
                args.fUniformHandler->getUniformCStr(fEllipseVar),
                fScaleVar.isValid() ? args.fUniformHandler->getUniformCStr(fScaleVar) : "half2(0)",
                args.fUniformHandler->getUniformCStr(fEllipseVar),
                fScaleVar.isValid() ? args.fUniformHandler->getUniformCStr(fScaleVar) : "half2(0)",
                _outer.edgeType(), args.fOutputColor,
                args.fInputColor ? args.fInputColor : "half4(1)");
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrEllipseEffect& _outer = _proc.cast<GrEllipseEffect>();
        auto edgeType = _outer.edgeType();
        (void)edgeType;
        auto center = _outer.center();
        (void)center;
        auto radii = _outer.radii();
        (void)radii;
        UniformHandle& ellipse = fEllipseVar;
        (void)ellipse;
        UniformHandle& scale = fScaleVar;
        (void)scale;

        if (radii != prevRadii || center != prevCenter) {
            float invRXSqd;
            float invRYSqd;
            // If we're using a scale factor to work around precision issues, choose the larger
            // radius as the scale factor. The inv radii need to be pre-adjusted by the scale
            // factor.
            if (scale.isValid()) {
                if (radii.fX > radii.fY) {
                    invRXSqd = 1.f;
                    invRYSqd = (radii.fX * radii.fX) / (radii.fY * radii.fY);
                    pdman.set2f(scale, radii.fX, 1.f / radii.fX);
                } else {
                    invRXSqd = (radii.fY * radii.fY) / (radii.fX * radii.fX);
                    invRYSqd = 1.f;
                    pdman.set2f(scale, radii.fY, 1.f / radii.fY);
                }
            } else {
                invRXSqd = 1.f / (radii.fX * radii.fX);
                invRYSqd = 1.f / (radii.fY * radii.fY);
            }
            pdman.set4f(ellipse, center.fX, center.fY, invRXSqd, invRYSqd);
            prevCenter = center;
            prevRadii = radii;
        }
    }
    SkPoint prevCenter;
    SkPoint prevRadii;
    bool useScale;
    UniformHandle fEllipseVar;
    UniformHandle fScaleVar;
};
GrGLSLFragmentProcessor* GrEllipseEffect::onCreateGLSLInstance() const {
    return new GrGLSLEllipseEffect();
}
void GrEllipseEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                            GrProcessorKeyBuilder* b) const {
    b->add32(fEdgeType);
}
bool GrEllipseEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrEllipseEffect& that = other.cast<GrEllipseEffect>();
    (void)that;
    if (fEdgeType != that.fEdgeType) return false;
    if (fCenter != that.fCenter) return false;
    if (fRadii != that.fRadii) return false;
    return true;
}
GrEllipseEffect::GrEllipseEffect(const GrEllipseEffect& src)
        : INHERITED(src.optimizationFlags())
        , fEdgeType(src.fEdgeType)
        , fCenter(src.fCenter)
        , fRadii(src.fRadii) {
    this->initClassID<GrEllipseEffect>();
}
std::unique_ptr<GrFragmentProcessor> GrEllipseEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrEllipseEffect(*this));
}
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrEllipseEffect);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrEllipseEffect::TestCreate(GrProcessorTestData* testData) {
    SkPoint center;
    center.fX = testData->fRandom->nextRangeScalar(0.f, 1000.f);
    center.fY = testData->fRandom->nextRangeScalar(0.f, 1000.f);
    SkScalar rx = testData->fRandom->nextRangeF(0.f, 1000.f);
    SkScalar ry = testData->fRandom->nextRangeF(0.f, 1000.f);
    GrPrimitiveEdgeType et;
    do {
        et = (GrPrimitiveEdgeType)testData->fRandom->nextULessThan(kGrProcessorEdgeTypeCnt);
    } while (kHairlineAA_GrProcessorEdgeType == et);
    return GrEllipseEffect::Make(et, center, SkPoint::Make(rx, ry));
}
#endif
#endif
