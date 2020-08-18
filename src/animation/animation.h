#pragma once


#include <../main.h>
#include "../objects/interfaces.h"

namespace CourseLab
{
	class AnimationLoder;
	class Gui;

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
		std::vector<QuatKey> m_rKeys;
		std::vector<VecKey> m_pKeys;

		glm::mat4 m_transform;

	public:

		inline Joint(const std::string& name, GLint id, glm::mat4 transform, glm::mat4 offset, Joint* parent = nullptr)
			: m_name(name), m_id(id), m_offset(offset), m_parent(parent)
		{
			/*if (m_parent != nullptr) 
				transform = m_parent->GetTransform() * transform;*/
			m_transform = transform;//glm::inverse(offset);

			m_sKeys.push_back({ 0.0f, glm::vec3(0.5f) });
			m_rKeys.push_back({ 0.0f, glm::angleAxis(glm::radians(-45.0f),glm::vec3(1.0f,0.0f,0.0f)) });
			/*m_pKeys.push_back({ 0.0f, glm::vec3(0.0f) });*/
		}
		inline ~Joint() {}

		inline void AddChild(const std::shared_ptr<Joint>& child) { 
			child->setParent(this);
			m_children.push_back(child); 
		}

		void GetCurrentPoseArray(float time, glm::mat4& bindTransform, 
			std::vector<glm::mat4>& poseTransforms, glm::mat4& InverseBindTransform) 
		{

			glm::mat4 localBindTransform;

			if (m_sKeys.size() == 0 && m_rKeys.size() == 0 && m_pKeys.size() == 0)
				localBindTransform = m_transform;
			else {
				localBindTransform = GetFrameTransform(time);
			}
			glm::mat4 poseBindTransform = bindTransform * localBindTransform;

			poseTransforms[m_id] = InverseBindTransform * poseBindTransform * m_offset;

			for (auto& joint : m_children) {
				joint->GetCurrentPoseArray(time, poseBindTransform, poseTransforms, InverseBindTransform);
			}
		}
		//inline void SetTransform(glm::mat4& m) { ; }

		glm::mat4 GetFrameTransform(float time) {
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetScaleVec(time));

			glm::mat4 rot = glm::toMat4(GetRotQuat(time));//glm::eulerAngleXYZ(ypr.y, ypr.x, ypr.z);

			glm::mat4 pos = glm::translate(glm::mat4(1.0f), GetPoseVec(time));
			
			return pos * rot * scale;
		}

		void setPoseVec(float time, glm::vec3& vec) {
			for (size_t i = 0; i + 1 < m_pKeys.size(); i++) {
				if (time < m_pKeys[i + 1].time) {
					m_pKeys.insert(m_pKeys.begin() + i + 1, { time,vec });
				}
				else if (time > m_pKeys[m_pKeys.size() - 1].time)
				{
					m_pKeys.push_back({ time,vec });
				}
			}
		}
		void setRotVec(float time, glm::vec3& vec) {
			for (size_t i = 0; i + 1 < m_rKeys.size(); i++) {
				if (time < m_rKeys[i + 1].time) {
					m_rKeys.insert(m_rKeys.begin() + i + 1, { time, glm::angleAxis(0.0f,vec) });
				}
				else if (time > m_rKeys[m_rKeys.size() - 1].time)
				{
					m_rKeys.push_back({ time,vec });
				}
			}
		}

		void setScaleVec(float time, glm::vec3& vec) {
			for (size_t i = 0; i + 1 < m_sKeys.size(); i++) {
				if (time < m_sKeys[i + 1].time) {
					__debugbreak();
					m_sKeys.insert(m_sKeys.begin() + i + 1, { time,vec });
				}
				else if (time > m_sKeys[m_sKeys.size() - 1].time)
				{
					m_sKeys.push_back({ time,vec });
				}
			}
		}

		glm::vec3 GetPoseVec(float time) {
			return interpolatePosKey(m_pKeys, time);
		}
		glm::quat GetRotQuat(float time) {
			return interpolateRotKey(m_rKeys, time);
		}
		glm::vec3 GetScaleVec(float time) {
			return interpolateScaleKey(m_sKeys, time);
		}

		inline glm::mat4 GetTransform()
		{
			return m_transform;
		}
	private:
		static glm::vec3 eulerAngles(const glm::quat& q) {
			return glm::normalize(
				  q * glm::vec3(0.0f, 1.0f, 0.0f)
				+ q * glm::vec3(1.0f, 0.0f, 0.0f)
				+ q * glm::vec3(0.0f, 0.0f, 1.0f)
			);
		}

		void setParent(Joint* parent) {
			this->m_parent = parent;
		}
		glm::vec3 interpolatePosKey(std::vector<VecKey>& keys, float time) {
			for (size_t i = 0; i + 1 < keys.size(); i++) {
				if (time < keys[i + 1].time) {
					float delta = keys[i + 1].time - keys[i].time;
					float factor = (time - keys[i].time) / delta;

					return glm::mix(keys[i].vector, keys[i + 1].vector, factor);
				}
				else if(time > keys[keys.size() - (size_t)1].time) {
					return keys[keys.size() - (size_t)1].vector;
				}
				else if (time < keys[0].time) {
					return keys[0].vector;
				}
			}
			if (keys.size() == 1)
				return keys[0].vector;
			else
				return glm::vec3(0.0f);
		}
		glm::vec3 interpolateScaleKey(std::vector<VecKey>& keys, float time) {
			for (size_t i = 0; i + 1 < keys.size() ; i++) {
				if (time < keys[i + 1].time) {
					float delta = keys[i + 1].time - keys[i].time;
					float factor = (time - keys[i].time) / delta;

					return glm::mix(keys[i].vector, keys[i + 1].vector, factor);
				}
				else if (time > keys[keys.size() - (size_t)1].time) {
					return keys[keys.size() - (size_t)1].vector;
				}
				else if (time < keys[0].time) {
					return keys[0].vector;
				}
			}
			if (keys.size() == 1)
				return keys[0].vector;
			else
				return glm::vec3(1.0f);
		}
		glm::quat interpolateRotKey(std::vector<QuatKey>& keys, float time) {
			for (size_t i = 0; i + 1 < keys.size(); i++) {
				if (time < keys[i + 1].time) {
					float delta = keys[i + 1].time - keys[i].time;
					float factor = (time - keys[i].time) / delta;

					auto quat1 = keys[i].quat;//glm::angleAxis(0.0f, keys[i].vector);
					auto quat2 = keys[i + 1].quat;//glm::angleAxis(0.0f, keys[i + 1].vector);

					return glm::slerp(quat1, quat2, factor);
				}
				else if (time > keys[keys.size() - (size_t)1].time) {
					return keys[keys.size() - (size_t)1].quat;
				}
				else if (time < keys[0].time) {
					return keys[0].quat;
				}
			}
			if (keys.size() == 1)
				return keys[0].quat;
			else
				return glm::vec3(0.0f);
		}

		friend class AnimationLoader;
		friend class Gui;
	};

	class AnimationLoader {
	private:
		const aiScene* m_scene;
		std::map<std::string, Bone> m_BoneMap;
		float m_ticksPerSec = 0.0f;
		float m_duration = 0;
	public:

		inline AnimationLoader(const aiScene* scene) : m_scene(scene) {}
		inline ~AnimationLoader() {}
		//call this for loop( vmesto getCurrentFrame() )
		inline float GetAnimTime(float time) {
			float ticks = time;
			return std::fmod(ticks, m_duration);
		}
		inline void SetBoneMap(std::map<std::string, Bone>& map) {
			m_BoneMap = map;
		}

		void LoadIntoBones(std::shared_ptr<Joint>& root, GLint id) {

			if (m_scene->HasAnimations()) {
				auto anim = m_scene->mAnimations[id];

				m_ticksPerSec = (float)(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 24.0f);
				m_duration = anim->mDuration * m_ticksPerSec;
				if (m_ticksPerSec * m_duration < 5)
				{
					m_ticksPerSec = 24.0f;
					m_duration *= m_ticksPerSec;
				}

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
					std::vector<Joint::QuatKey> rKeys;
					std::vector<Joint::VecKey> sKeys;

					for (GLuint j = 0; j < channel->mNumPositionKeys; j++) {
						pKeys.push_back({ (float)channel->mPositionKeys[j].mTime * m_ticksPerSec,
							vec3_cast(channel->mPositionKeys[j].mValue) });
					}

					for (GLuint j = 0; j < channel->mNumRotationKeys; j++) {

						rKeys.push_back({ (float)channel->mRotationKeys[j].mTime * m_ticksPerSec,
							eulerAngles(quat_cast(channel->mRotationKeys[j].mValue)) });
					}

					for (GLuint j = 0; j < channel->mNumScalingKeys; j++) {
						sKeys.push_back({ (float)channel->mScalingKeys[j].mTime * m_ticksPerSec,
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