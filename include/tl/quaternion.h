#pragma once
#include "math.h"

namespace tl {

union quaternion {
	struct { f32 x, y, z, w; };
	v3f xyz;
	f32 s[4];
	quaternion operator-() const {
		return {-x, -y, -z, w};
	}
	v3f operator*(v3f v) const {
		return v + 2 * cross(xyz, w * v + cross(xyz, v));// / (x*x+y*y+z*z+w*w);
	}
	explicit operator m3() const {
		return {
			1 - 2 * (y*y + z*z),     2 * (x*y - z*w),     2 * (x*z + y*w),
				2 * (x*y + z*w), 1 - 2 * (x*x + z*z),     2 * (y*z - x*w),
				2 * (x*z - y*w),     2 * (y*z + x*w), 1 - 2 * (x*x + y*y),
		};
	}
	explicit operator m4() const {
		return to_m4(operator m3());
	}
	static quaternion identity() {
		return {0,0,0,1};
	}
	quaternion operator*(quaternion b) const {
		quaternion r;
		r.w = w * b.w - dot(xyz, b.xyz);
		r.xyz = w * b.xyz + b.w * xyz + cross(xyz, b.xyz);
		return r;
	}
	quaternion &operator*=(quaternion b) { return *this = *this * b, *this; }
};

quaternion Quaternion(v3f xyz, f32 w) {
	return {xyz.x, xyz.y, xyz.z, w};
}

quaternion normalize(quaternion q) {
	f32 il = 1 / length(v4f{q.x,q.y,q.z,q.w});
	return {q.x*il,q.y*il,q.z*il,q.w*il};
}

quaternion quaternion_from_axis_angle(v3f axis, f32 angle) {
	f32 half_angle = angle * 0.5f;
	f32 s = tl::sin(half_angle);
	return {
		axis.x * s,
		axis.y * s,
		axis.z * s,
		tl::cos(half_angle),
	};
}
quaternion quaternion_from_euler(f32 ax, f32 ay, f32 az) {
	v3f half_angle = v3f{ax, ay, az} * 0.5f;
	v2f csx = cos_sin(half_angle.x);
	v2f csy = cos_sin(half_angle.y);
	v2f csz = cos_sin(half_angle.z);

	f32 a = csx.x;
	f32 b = csx.y;
	f32 c = csy.x;
	f32 d = csy.y;
	f32 e = csz.x;
	f32 f = csz.y;

	return {
		e*c*b + a*d*f,
		-c*b*f + e*a*d,
		c*a*f + -e*d*b,
		c*a*e + d*b*f
	};

	/*
	forceinline v3f cross(v3f a, v3f b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
	*/

	return Quaternion(
		v3f{csy.x*csx.y,csx.x*csy.y,-csy.y*csx.y},
		csy.x * csx.x
	) * quaternion {
		0,
		0,
		csz.y,
		csz.x,
	};

	return quaternion {
		0,
		csy.y,
		0,
		csy.x,
	} * quaternion {
		csx.y,
		0,
		0,
		csx.x,
	} * quaternion {
		0,
		0,
		csz.y,
		csz.x,
	};

	return quaternion_from_axis_angle({0,1,0}, ay) * quaternion_from_axis_angle({1,0,0}, ax) * quaternion_from_axis_angle({0,0,1}, az);

	v2f z = cos_sin(ax * -0.5f);
	v2f x = cos_sin(ay * -0.5f);
	v2f y = cos_sin(az * -0.5f);

	quaternion q;
	q.x = y.x * x.x * z.y - y.y * x.y * z.x;
	q.y = y.x * x.y * z.x + y.y * x.x * z.y;
	q.z = y.y * x.x * z.x - y.x * x.y * z.y;
	q.w = y.y * x.y * z.y + y.x * x.x * z.x;
	return q;
}
quaternion quaternion_from_euler(v3f v) {
	return quaternion_from_euler(v.x, v.y, v.z);
}
#if 0
quaternion quaternion_look(v3f direction, v3f up) {
	v3f the_forward = {0,0,-1};
	v3f forward = normalize(direction);

	f32 d = dot(the_forward, forward);

	if (absolute(d - (-1.0f)) < 0.000001f) {
		return normalize(quaternion{up.x, up.y, up.z, pi});
	}
	if (absolute(d - (1.0f)) < 0.000001f) {
		return quaternion::identity();
	}

	f32 rotAngle = (f32)acos(d);
	v3f rotAxis = cross(the_forward, forward);
	rotAxis = normalize(rotAxis);
	return quaternion_from_axis_angle(rotAxis, rotAngle);
}
#elif 0
quaternion quaternion_look(v3f direction, v3f up) {
	/*v3f forward = direction.Normalized();
	v3f right = v3f::Cross(up.Normalized(), forward);
	v3f up = v3f::Cross(forward, right);*/

	up = normalize(up);
	v3f forward = normalize(direction);
	v3f right = normalize(cross(up, forward));
	forward = cross(right, up);
	//v3f::OrthoNormalize(&up, &forward); // Keeps up the same, make forward orthogonal to up

	quaternion ret;
	ret.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
	float w4_recip = 1.0f / (4.0f * ret.w);
	ret.x = (forward.y - up.z) * w4_recip;
	ret.y = (right.z - forward.x) * w4_recip;
	ret.z = (up.x - right.y) * w4_recip;

	return ret;
}
#else
quaternion quaternion_look(v3f direction) {
	quaternion ry = quaternion_from_axis_angle({0,1,0}, atan2(direction.xz()));
	quaternion rz = quaternion_from_axis_angle({0,0,1}, -atan2((ry * direction).xy));
	return rz * ry;
}
#endif

v3f to_euler_angles(quaternion q) {
	// Store the Euler angles in radians
    v3f pitchYawRoll;

    f32 sqw = q.w * q.w;
    f32 sqx = q.x * q.x;
    f32 sqy = q.y * q.y;
    f32 sqz = q.z * q.z;

    // If quaternion is normalised the unit is one, otherwise it is the correction factor
    f32 unit = sqx + sqy + sqz + sqw;
    f32 test = q.x * q.y + q.z * q.w;

    if (test > 0.4999f * unit)                              // 0.4999f OR 0.5f - EPSILON
    {
        // Singularity at north pole
        pitchYawRoll.y = 2 * tl::atan2(q.x, q.w);  // Yaw
        pitchYawRoll.x = pi * 0.5f;                         // Pitch
        pitchYawRoll.z = 0;                                // Roll
    }
    else if (test < -0.4999f * unit)                        // -0.4999f OR -0.5f + EPSILON
    {
        // Singularity at south pole
        pitchYawRoll.y = -2 * tl::atan2(q.x, q.w); // Yaw
        pitchYawRoll.x = -pi * 0.5f;                        // Pitch
        pitchYawRoll.z = 0;                                // Roll
    }
    else
    {
        pitchYawRoll.y = tl::atan2(2 * q.y * q.w - 2 * q.x * q.z, sqx - sqy - sqz + sqw);       // Yaw
        pitchYawRoll.x = asin(2 * test / unit);                                             // Pitch
        pitchYawRoll.z = tl::atan2(2 * q.x * q.w - 2 * q.y * q.z, -sqx + sqy - sqz + sqw);      // Roll
    }

	return {
		-tl::atan2(2*(q.w*q.x + q.y*q.z), 1 - 2*(q.x*q.x + q.y*q.y)),
		-asinf(2*(q.w*q.y - q.z*q.x)),
		-tl::atan2(2*(q.w*q.z + q.x*q.y), 1 - 2*(q.y*q.y + q.z*q.z)),
	};
    return pitchYawRoll;
}

void append(StringBuilder &builder, quaternion q) {
	append_format(builder, "{%, %, %, %}", q.x, q.y, q.z, q.w);
}

}
