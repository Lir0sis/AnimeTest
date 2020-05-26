#pragma once


#include <../main.h>
#include "../objects/interfaces.h"

namespace CourseLab
{
	class AnimationLoder;

	class Joint {
	public:
		struct VecKey {
			float time;
			glm::vec3 vector;
		};
		struct QuatKey {
			float time;
			glm::quat quat;
		};

	private:
		std::string m_name;
		GLint m_id;
		glm::mat4 m_offset;

		Joint* m_parent;
		std::vector<std::shared_ptr<Joint>> m_children;
	
		std::vector<VecKey> m_sKeys;
		std::vector<VecKey> m_rKeys;
		std::vector<VecKey> m_pKeys;

		glm::vec3 m_pos;
		glm::vec3 m_rot;
		glm::vec3 m_scale;

	public:

		inline Joint(const std::string& name, GLint id, glm::mat4 transform, glm::mat4 offset, Joint* parent = nullptr)
			: m_name(name), m_id(id), m_offset(offset), m_parent(parent)
		{
			decompose(transform, m_pos, m_rot, m_scale);

			m_sKeys.push_back({ 0.0f, m_scale });
			m_rKeys.push_back({ 0.0f, m_rot });
			m_pKeys.push_back({ 0.0f, m_pos });
		}
		inline ~Joint() {}

		inline void AddChild(const std::shared_ptr<Joint>& child) { 
			child->setParent(this);
			m_children.push_back(child); }

		void GetCurrentPoseArray(float time, glm::mat4& bindTransform, std::vector<glm::mat4>& poseTransforms, glm::mat4& InverseBindTransform) {

			glm::mat4 localBindTransform;

			if (m_sKeys.size() == 0 && m_rKeys.size() == 0 && m_pKeys.size() == 0)
				localBindTransform = GetTransform();
			else {

				glm::mat4 scale = glm::scale(glm::mat4(1.0f), interpolateKey(m_sKeys, time));
				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), 0.0f, interpolateRotKey(m_rKeys, time));
				glm::mat4 pos = glm::translate(glm::mat4(1.0f), interpolateKey(m_pKeys, time));

				localBindTransform = pos * rot * scale;
			}
			glm::mat4 poseBindTransform = bindTransform * localBindTransform;

			poseTransforms[m_id] = InverseBindTransform * poseBindTransform * m_offset;

			for (auto& joint : m_children) {
				joint->GetCurrentPoseArray(time, poseBindTransform, poseTransforms, InverseBindTransform);
			}
		}
		inline void SetTransform(glm::mat4& m) { decompose(m, m_pos, m_rot, m_scale); }
		inline glm::mat4 GetTransform() 
		{ 
			return glm::translate(glm::mat4(1.0f), m_pos)
				* glm::toMat4(glm::rotate(glm::angleAxis(0.0f, m_rot), 0.0f, glm::vec3(0.0f)))
				* glm::scale(glm::mat4(1.0f), m_scale);
		}

		glm::mat4 GetFrameTransform(float time) {
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), interpolateKey(m_sKeys, time));
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f),0.0f, interpolateKey(m_rKeys, time));
			glm::mat4 pos = glm::translate(glm::mat4(1.0f), interpolateKey(m_pKeys, time));
			
			return pos * rot * scale;
		}

		glm::vec3 GetPoseVec(float time) {
			return interpolateKey(m_pKeys, time);
		}
		glm::vec3 GetRotVec(float time) {
			return interpolateRotKey(m_rKeys, time);
		}
		glm::vec3 GetScaleVec(float time) {
			return interpolateKey(m_sKeys, time);
		}

	private:
		void decompose(glm::mat4 m, glm::vec3& p, glm::vec3& r, glm::vec3& s) {

			glm::vec3 scale;
			glm::quat rot;
			glm::vec3 pos;
			glm::vec3 skew; //dummy
			glm::vec4 perspective; //dummy

			glm::decompose(m, scale, rot, pos, skew, perspective);

			p = pos; r = glm::eulerAngles(rot); s = scale;
		}

		void setParent(Joint* parent) {
			this->m_parent = parent;
		}
		glm::vec3 interpolateKey(std::vector<VecKey>& keys, float time) {
			for (GLint i = 0; i < keys.size() - 1; i++) {
				if (time < keys[i + 1].time) {
					float delta = keys[i + 1].time - keys[i].time;
					float factor = (time - keys[i].time) / delta;

					return glm::mix(keys[i].vector, keys[i + 1].vector, factor);
				}
			}
			return keys[0].vector;
		}
		glm::vec3 interpolateRotKey(std::vector<VecKey>& keys, float time) {
			for (GLint i = 0; i < keys.size() - 1; i++) {
				if (time < keys[i + 1].time) {
					float delta = keys[i + 1].time - keys[i].time;
					float factor = (time - keys[i].time) / delta;

					auto quat1 = glm::angleAxis(0.0f, keys[i].vector);
					auto quat2 = glm::angleAxis(0.0f, keys[i + 1].vector);

					return glm::eulerAngles(glm::slerp(quat1, quat2, factor));
				}
			}
			return keys[0].vector;
		}

		friend class AnimationLoader;
	};

	class AnimationLoader {
	private:
		const aiScene* m_scene;
		std::map<std::string, Bone> m_BoneMap;
		float m_ticksPerSec = 0.0f;
		float m_duration = 0.0f;
	public:

		inline AnimationLoader(const aiScene* scene) : m_scene(scene) {}
		inline ~AnimationLoader() {}

		inline float GetAnimTime(float time) {
			float ticks = time * m_ticksPerSec;
			return std::fmod(ticks, m_duration);
		}
		inline void SetBoneMap(std::map<std::string, Bone>& map) {
			m_BoneMap = map;
		}

		void LoadIntoBones(std::shared_ptr<Joint>& root, GLint id) {

			if (m_scene->HasAnimations()) {
				auto anim = m_scene->mAnimations[id];

				m_ticksPerSec = (float)(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 24.0f);
				m_duration = anim->mDuration;

				setKeys(anim, root);
			}
		}
	private:
		void setKeys(const aiAnimation* anim, std::shared_ptr<Joint>& joint)
		{
			for (GLuint i = 0; i < anim->mNumChannels; i++) {
				auto channel = anim->mChannels[i];
				if (std::string(channel->mNodeName.data) == joint->m_name) {

					std::vector<Joint::VecKey> pKeys;
					std::vector<Joint::VecKey> rKeys;
					std::vector<Joint::VecKey> sKeys;

					for (GLuint j = 0; j < channel->mNumPositionKeys; j++) {
						pKeys.push_back({ (float)channel->mPositionKeys[j].mTime,
							vec3_cast(channel->mPositionKeys[j].mValue) });
					}

					for (GLuint j = 0; j < channel->mNumRotationKeys; j++) {
						rKeys.push_back({ (float)channel->mRotationKeys[j].mTime,
							glm::eulerAngles(quat_cast(channel->mRotationKeys[j].mValue)) });
					}

					for (GLuint j = 0; j < channel->mNumScalingKeys; j++) {
						sKeys.push_back({ (float)channel->mScalingKeys[j].mTime,
							vec3_cast(channel->mScalingKeys[j].mValue) });
					}

					joint->m_pKeys = pKeys;
					joint->m_rKeys = rKeys;
					joint->m_sKeys = sKeys;
				}
			}

			for (auto& child : joint->m_children) {
				setKeys(anim, child);
			}
		}

	};


	/*class JointFrame : public virtual Transform {
	private:
		float m_Time;
	public:
		inline JointFrame(float time) : Transform(), m_Time(time) {}
		inline float GetTime() { return m_Time; }
	};*/
}