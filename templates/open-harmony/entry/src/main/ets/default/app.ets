import nativerender from "libcocos2d.so";
import { ContextType } from "./common/Constants"
import { WorkerManager } from './cocos/WorkerManager.ets'
import {log} from './workers/log_utils'


log('===========================================')

const nativeAppLifecycle = nativerender.getContext(ContextType.APP_LIFECYCLE);
export default {
  onCreate() {
    console.info('[LIFECYCLE-App] kee cocos onCreate')
    /***App 生命周期回调在Host JS Runtime中完成***/
    nativeAppLifecycle.onCreate();

  },
  onShow() {
    console.info('[LIFECYCLE-App] kee cocos onShow')
    nativeAppLifecycle.onShow();
  },
  onHide() {
    console.info('[LIFECYCLE-App] kee cocos onHide')
    nativeAppLifecycle.onHide();
  },
  onDestroy() {
    console.info('[LIFECYCLE-App] kee cocos onDestroy')
    nativeAppLifecycle.onDestroy();
  },
}