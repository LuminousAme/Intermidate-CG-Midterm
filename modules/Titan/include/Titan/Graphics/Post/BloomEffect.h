//Titan Engine by Atlas X Games
//File made by Ame Gilham for Intermdiate CG midterm 
//BloomEffect.h - Header for the class for bloom post processing effects
#pragma once

//include the precompile header with a bunch of system stuff, as well as the base post effect class
#include "Titan/ttn_pch.h"
#include "Titan/Graphics/Post/PostEffect.h"

namespace Titan {
	//class for color correction post processing effects
	class TTN_BloomEffect : public TTN_PostEffect
	{
	public:
		//defines a special easier to use name for shared(smart) pointers to the class 
		typedef std::shared_ptr<TTN_BloomEffect> sbloomptr;

		//creates and returns a shared(smart) pointer to the class 
		static inline sbloomptr Create() {
			return std::make_shared<TTN_BloomEffect>();
		}

	public:
		//Init framebuffer
		void Init(unsigned width, unsigned height) override;

		//Applies effect to this buffer
		//passes the previous framebuffer with the texture to apply as a parameter
		void ApplyEffect(TTN_PostEffect::spostptr buffer) override;

		//Getters
		float GetThreshold() { return m_threshold; }
		int GetNumOfPasses() { return m_numOfPasses; }
		//Setters
		void SetThreshold(float threshold) { m_threshold = threshold; }
		void SetNumOfPasses(int numOfPasses) { m_numOfPasses = numOfPasses; }

	private:
		//the threshold for how much of the bright colours to extract
		float m_threshold;
		//the number of blur passes that should be performed
		int m_numOfPasses;
	};
}
