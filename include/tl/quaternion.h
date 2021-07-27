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
			*this * v3f{1, 0, 0},
			*this * v3f{0, 1, 0},
			*this * v3f{0, 0, 1},
		};
		//return {
		//	1 - 2 * (y*y + z*z),     2 * (x*y - z*w),     2 * (x*z + y*w),
		//		2 * (x*y + z*w), 1 - 2 * (x*x + z*z),     2 * (y*z - x*w),
		//		2 * (x*z - y*w),     2 * (y*z + x*w), 1 - 2 * (x*x + y*y),
		//};
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
		e*a*d - c*b*f,
		c*a*f - e*d*b,
		c*a*e + d*b*f,
	};
}
quaternion quaternion_from_euler(v3f v) {
	return quaternion_from_euler(v.x, v.y, v.z);
}

quaternion quaternion_look(v3f direction) {
	quaternion ry = quaternion_from_axis_angle({0,1,0}, -atan2(direction.xz()));
	quaternion rz = quaternion_from_axis_angle({0,0,1}, atan2((-ry * direction).xy));
	return ry * rz;
}

v3f to_euler_angles(quaternion q) {
	v3f angles = {};
	v3f rotated_x = q * v3f{1, 0, 0};
	v3f rotated_z = q * v3f{0, 0, 1};
	angles.y = atan2(rotated_z.zx());

	q = quaternion_from_axis_angle({0, 1, 0}, -angles.y);
	rotated_x = q * rotated_x;
	rotated_z = q * rotated_z;
	angles.x = -atan2(rotated_z.zy());
	
	q = quaternion_from_axis_angle({1, 0, 0}, -angles.x);
	rotated_x = q * rotated_x;
	rotated_z = q * rotated_z;

	angles.z = atan2(rotated_x.xy);

	return angles;
}

void append(StringBuilder &builder, quaternion q) {
	append_format(builder, "{%, %, %, %}", q.x, q.y, q.z, q.w);
}

}
