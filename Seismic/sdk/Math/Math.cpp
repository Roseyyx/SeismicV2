#include "Math.h"
#include "../Helpers/Interfaces.h"


bool M::Setup()
{
	const void* hVstdLib = MEM::GetModuleBaseHandle("vstdlib.dll");

	if (hVstdLib == nullptr)
		return false;

	RandomSeed = reinterpret_cast<RandomSeedFn>(MEM::GetExportAddress(hVstdLib, "RandomSeed"));
	if (RandomSeed == nullptr)
		return false;

	RandomFloat = reinterpret_cast<RandomFloatFn>(MEM::GetExportAddress(hVstdLib, "RandomFloat"));
	if (RandomFloat == nullptr)
		return false;

	RandomFloatExp = reinterpret_cast<RandomFloatExpFn>(MEM::GetExportAddress(hVstdLib, "RandomFloatExp"));
	if (RandomFloatExp == nullptr)
		return false;

	RandomInt = reinterpret_cast<RandomIntFn>(MEM::GetExportAddress(hVstdLib, "RandomInt"));
	if (RandomInt == nullptr)
		return false;

	RandomGaussianFloat = reinterpret_cast<RandomGaussianFloatFn>(MEM::GetExportAddress(hVstdLib, "RandomGaussianFloat"));
	if (RandomGaussianFloat == nullptr)
		return false;

	return true;
}

void M::VectorAngles(const Vector& vecForward, QAngle& angView)
{
	float flPitch, flYaw;

	if (vecForward.x == 0.f && vecForward.y == 0.f)
	{
		flPitch = (vecForward.z > 0.f) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f(-vecForward.z, vecForward.Length2D()) * 180.f / M_PI;

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = std::atan2f(vecForward.y, vecForward.x) * 180.f / M_PI;

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	angView.x = flPitch;
	angView.y = flYaw;
	angView.z = 0.f;
}

void M::AngleVectors(const QAngle& angView, Vector* pForward, Vector* pRight, Vector* pUp)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView.z));

	if (pForward != nullptr)
	{
		pForward->x = cp * cy;
		pForward->y = cp * sy;
		pForward->z = -sp;
	}

	if (pRight != nullptr)
	{
		pRight->x = -1 * sr * sp * cy + -1 * cr * -sy;
		pRight->y = -1 * sr * sp * sy + -1 * cr * cy;
		pRight->z = -1 * sr * cp;
	}

	if (pUp != nullptr)
	{
		pUp->x = cr * sp * cy + -sr * -sy;
		pUp->y = cr * sp * sy + -sr * cy;
		pUp->z = cr * cp;
	}
}

void M::AngleMatrix(const QAngle& angView, matrix3x4_t& matOutput, const Vector& vecOrigin)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView.z));

	matOutput.SetForward(Vector(cp * cy, cp * sy, -sp));

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	matOutput.SetLeft(Vector(sp * srcy - crsy, sp * srsy + crcy, sr * cp));
	matOutput.SetUp(Vector(sp * crcy + srsy, sp * crsy - srcy, cr * cp));
	matOutput.SetOrigin(vecOrigin);
}

Vector2D M::AnglePixels(const float flSensitivity, const float flPitch, const float flYaw, const QAngle& angBegin, const QAngle& angEnd)
{
	QAngle angDelta = angBegin - angEnd;
	angDelta.Normalize();

	const float flPixelMovePitch = (-angDelta.x) / (flYaw * flSensitivity);
	const float flPixelMoveYaw = (angDelta.y) / (flPitch * flSensitivity);

	return Vector2D(flPixelMoveYaw, flPixelMovePitch);
}

QAngle M::PixelsAngle(const float flSensitivity, const float flPitch, const float flYaw, const Vector2D& vecPixels)
{
	const float flAngleMovePitch = (-vecPixels.x) * (flYaw * flSensitivity);
	const float flAngleMoveYaw = (vecPixels.y) * (flPitch * flSensitivity);

	return QAngle(flAngleMoveYaw, flAngleMovePitch, 0.f);
}

QAngle M::CalcAngle(const Vector& vecStart, const Vector& vecEnd)
{
	QAngle angView;
	const Vector vecDelta = vecEnd - vecStart;
	VectorAngles(vecDelta, angView);
	angView.Normalize();

	return angView;
}

Vector M::VectorTransform(const Vector& vecTransform, const matrix3x4_t& matrix)
{
	return Vector(vecTransform.DotProduct(matrix[0]) + matrix[0][3],
		vecTransform.DotProduct(matrix[1]) + matrix[1][3],
		vecTransform.DotProduct(matrix[2]) + matrix[2][3]);
}

Vector M::ExtrapolateTick(const Vector& p0, const Vector& v0)
{
	// position formula: p0 + v0t
	return p0 + (v0 * Interfaces::GlobalVars->flIntervalPerTick);
}

bool M::IntersectLineWithBB(Vector& vecStart, Vector& vecEnd, Vector& vecMin, Vector& vecMax) {
	char quadrant[3];
	int i;

	Vector candidatePlane;
	for (i = 0; i < 3; i++) {
		if (vecStart[i] < vecMin[i]) {
			quadrant[i] = 1;
			candidatePlane[i] = vecMin[i];
		}
		else if (vecStart[i] > vecMax[i]) {
			quadrant[i] = 0;
			candidatePlane[i] = vecMax[i];
		}
		else {
			quadrant[i] = 2;
		}
	}

	// Calculate T distances to candidate planes
	Vector maxT;
	for (i = 0; i < 3; i++) {
		if (quadrant[i] != 2 && vecEnd[i] != 0.f)
			maxT[i] = (candidatePlane[i] - vecStart[i]) / vecEnd[i];
		else
			maxT[i] = -1.f;
	}

	// Get largest of the maxT's for final choice of intersection
	int whichPlane = 0;
	for (i = 1; i < 3; i++) {
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;
	}

	if (maxT[whichPlane] < 0.0f)
		return false;

	for (i = 0; i < 3; i++) {
		if (whichPlane != i) {
			float temp = vecStart[i] + maxT[whichPlane] * vecEnd[i];
			if (temp < vecMin[i] || temp > vecMax[i]) {
				return false;
			}
		}
	}

	return true;
}

void M::VectorTransformI(const Vector& vecTransform, const matrix3x4_t& matrix, Vector& Out) {
	Out.x = vecTransform.DotProduct(matrix[0]) + matrix[0][3];
	Out.y = vecTransform.DotProduct(matrix[1]) + matrix[1][3];
	Out.z = vecTransform.DotProduct(matrix[2]) + matrix[2][3];
}

float M::NormalizeYaw(float f) {
	if (std::isnan(f) || std::isinf(f))
		f = 0.0f;

	if (f > 9999999.0f)
		f = 0.0f;

	if (f < -9999999.0f)
		f = 0.0f;

	while (f < -180.0f)
		f += 360.0f;

	while (f > 180.0f)
		f -= 360.0f;

	return f;
}

float M::NormalizePitch(float f) {
	while (f > 89.0f)
		f -= 180.0f;

	while (f < -89.0f)
		f += 180.0f;

	return f;
}