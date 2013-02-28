package com.bigcloud
{
	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class bigcloud
	{
		private static var instance:bigcloud;
		private static var allowInstantiation:Boolean;

		private var unsentSessionLength:Number;
		private var timer:Timer;
		private var lastTime:Number;
		private var isSuspended:Boolean;
		private var eventQueue:EventQueue;

		public var version:String;

		public function bigcloud ()
		{
			if ( !allowInstantiation )
				throw new Error( "Error: Call bigcloud.sharedInstance() to use this singleton" );

			// Init
			timer               = null;
			unsentSessionLength = 0;
			isSuspended         = false;
			eventQueue          = new EventQueue();

			//
			// -- didEnterForegroundCallBack
			// -- If you are developing with air for native environment, uncomment next line:
			// NativeApplication.nativeApplication.addEventListener(Event.ACTIVATE, didEnterForegroundCallBack);
			//
			// -- If you are developing for web/browsers, uncomment next line
			bigcloudParse.addExternalEventListener("window.onfocus", this.didEnterForegroundCallBack, "didEnterForegroundCallBack");
			//

			//
			// -- didEnterBackgroundCallBack
			// -- If you are developing with air for native environment, uncomment next line:
			// NativeApplication.nativeApplication.addEventListener(Event.DEACTIVATE, didEnterBackgroundCallBack);
			//
			// -- If you are developing for web/browsers, uncomment next line
			bigcloudParse.addExternalEventListener("window.onblur", this.didEnterBackgroundCallBack, "didEnterBackgroundCallBack");
			//

			//
			// -- willTerminateCallBack
			// -- If you are developing with air for native environment, uncomment next line:
			// NativeApplication.nativeApplication.addEventListener(Event.CLOSING, willTerminateCallBack);
			//
			// -- If you are developing for web/browsers, uncomment next line
			// -- Event will be captured, but data send may not finish before the browser closes.
			bigcloudParse.addExternalEventListener("window.onbeforeunload", this.willTerminateCallBack, "willTerminateCallBack");
		}

		public static function sharedInstance ():bigcloud
		{
			if ( instance == null ) {
				allowInstantiation = true;
				instance           = new bigcloud();
				allowInstantiation = false;
			}
			return instance;
		}

		public function start(appKey:String, appVersion:String, appHost:String):void
		{
			version = appVersion;

			timer = new Timer(30*1000, 0);
			timer.addEventListener(TimerEvent.TIMER, onTimer);

			lastTime = bigcloudParse.unixTime();

			ConnectionQueue.sharedInstance().appKey  = appKey;
			ConnectionQueue.sharedInstance().appHost = appHost;
			ConnectionQueue.sharedInstance().beginSession();

			timer.start();
		}

		public function recordEvent(key:String, count:int, sum:Number = 0):void
		{
			eventQueue.recordEvent(key, count, sum);
			if(eventQueue.count >= 5) {
				ConnectionQueue.sharedInstance().recordEvents(eventQueue.events());
			}
		}

		public function recordEventSegmentation(key:String, segmentationKey:String, segmentationValue:String, count:int, sum:Number = 0):void
		{
			eventQueue.recordEventSegmentation(key, segmentationKey, segmentationValue, count, sum);
			if(eventQueue.count >= 5) {
				ConnectionQueue.sharedInstance().recordEvents(eventQueue.events());
			}
		}

		private function onTimer(event:TimerEvent):void
		{
			if(isSuspended) {
				return;
			}

			var currTime:Number = bigcloudParse.unixTime();
			unsentSessionLength += currTime - lastTime;
			lastTime = currTime;

			var duration:int = unsentSessionLength;
			ConnectionQueue.sharedInstance().updateSessionWithDuration(duration);
			unsentSessionLength -= duration;

			if(eventQueue.count > 0) {
				ConnectionQueue.sharedInstance().recordEvents(eventQueue.events());
			}
		}

		private function suspend():void
		{
			isSuspended = true;

			var currTime:Number = bigcloudParse.unixTime();
			unsentSessionLength += currTime - lastTime;

			var duration:int = unsentSessionLength;
			ConnectionQueue.sharedInstance().endSessionWithDuration(duration);
			unsentSessionLength -= duration;
		}

		private function resume():void
		{
			lastTime = bigcloudParse.unixTime();
			ConnectionQueue.sharedInstance().beginSession();
			isSuspended = false;
		}

		private function exit():void
		{
			//
		}

		private function didEnterBackgroundCallBack(event:* = false):void
		{
			this.suspend();
		}

		private function didEnterForegroundCallBack(event:* = false):void
		{
			this.resume();
		}

		private function willTerminateCallBack(event:* = false):void
		{
			this.exit();
		}
	}
}

