#include "stdafx.h"
#include "NTimerSrvImpl.h"

namespace nui
{
    namespace Ui
    {

        IMPLEMENT_REFLECTION_EX(NTimerSrvImpl, Base::NReflect::Singleton);

        NTimerSrvImpl::NTimerSrvImpl()
        {
            baseTimerId_ = 1;
            window_.Create(HWND_MESSAGE);
            NAssertError(window_.GetNative() != NULL, _T("Failed to create window"));
            window_.SetMsgFilterCallback(MakeDelegate(this, &NTimerSrvImpl::TimerWindowProc));
        }

        NTimerSrvImpl::~NTimerSrvImpl()
        {
            if(window_.GetNative() != NULL)
                window_.Destroy();
        }

        Base::NHolder NTimerSrvImpl::startTimer(int elapse, NTimerCallback callback)
        {
            return startTimerImpl(elapse, callback, false);
        }

        Base::NHolder NTimerSrvImpl::startOnceTimer(int elapse, NTimerCallback callback)
        {
            return startTimerImpl(elapse, callback, true);
        }

        void NTimerSrvImpl::ReleaseTimer(void* data)
        {
            UINT_PTR timerId = reinterpret_cast<UINT_PTR>(data);
            if(timerId <= 0)
                return;
            nui::Base::NInstPtr<NTimerSrv> timerSrv(MemToolParam);
            if(timerSrv == NULL)
                return;
            NTimerSrvImpl* impl = dynamic_cast<NTimerSrvImpl*>((NTimerSrv*)timerSrv);
            impl->stopTimer(timerId);
        }

        UINT_PTR NTimerSrvImpl::GetNextTimerId()
        {
            return baseTimerId_++;
        }

        Base::NHolder NTimerSrvImpl::startTimerImpl(int elapse, NTimerCallback callback, bool isOnce)
        {
            NAssertError(window_.GetNative() != NULL, _T("No window could be used"));
            UINT_PTR timerId = ::SetTimer(window_.GetNative(), GetNextTimerId(), elapse, NULL);
            NAssertError(timerId > 0, _T("Failed to create timer"));
            TimerData data;
            data.isOnceTimer = isOnce;
            data.callback = callback;
            timerMap_.insert(std::make_pair(timerId, data));

            Base::NHolder holder(reinterpret_cast<void*>(timerId), &NTimerSrvImpl::ReleaseTimer);
            return holder;
        }

        void NTimerSrvImpl::stopTimer(UINT_PTR timerId)
        {
            TimerMap::iterator ite = timerMap_.find(timerId);
            if(ite == timerMap_.end())
                return;
            timerMap_.erase(ite);
            ::KillTimer(window_.GetNative(), timerId);
        }

        bool NTimerSrvImpl::TimerWindowProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            UNREFERENCED_PARAMETER(lParam);
            UNREFERENCED_PARAMETER(lResult);
            if(message != WM_TIMER)
                return false;
            TimerMap::iterator ite = timerMap_.find(wParam);
            if(ite == timerMap_.end())
                return false;
            TimerData& data = ite->second;
            if(data.isOnceTimer)
                ::KillTimer(window->GetNative(), wParam);
            NTimerCallback callback = data.callback;
            if(data.isOnceTimer)
                timerMap_.erase(ite);
            callback();
            return true;
        }
    }
}