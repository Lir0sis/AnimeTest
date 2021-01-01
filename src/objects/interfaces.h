#pragma once

#include <main.h>

namespace CourseLab
{
	class Object
	{
	private:
		const int m_ID;
		static int Counter() { static int ID{ 0 }; return ID++; }
	public:
		inline Object() : m_ID(Counter()) {}
		virtual ~Object() {};

		inline const int GetID() const { return m_ID; }

		virtual void Update() = 0;
	};

	class Transform
	{
	private:
		glm::vec3 m_pos;
		glm::vec3 m_rot;
		glm::vec3 m_scale;
	public:
		Transform();
		~Transform();

		inline glm::vec3& Pos() { return m_pos; }
		inline glm::vec3& Rot() { return m_rot; }
		inline glm::vec3& Size() { return m_scale; }

		inline void Pos(const glm::vec3& pos) { m_pos = pos; }
		inline void Rot(const glm::vec3& rot) { m_rot = rot; }
		inline void Size(const glm::vec3& scale) { m_scale = scale; }

		const glm::mat4 GetTransform();

		inline void Translate(const glm::vec3& pos) { m_pos += pos; }
		inline void Rotate(const glm::vec3& rot) { m_rot += rot; }
		inline void Scale(const glm::vec3& scale) { m_scale *= scale; }

	};
	struct Bone
	{
		GLint id;
		glm::mat4 offset;
	};

	static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
	static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); }
	static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
	static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }

	/*class IKeyFrame {
	private:

	public:

		virtual void Interpolate(){}
	};*/
}
