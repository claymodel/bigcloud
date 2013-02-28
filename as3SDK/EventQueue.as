package com.bigcloud
{
	public class EventQueue
	{
		private var events_:Array;

		public function EventQueue ()
		{
			events_ = [];
		}

		public function get count():uint
		{
			return events_.length;
		}

		public function events():String
		{
			var result:String = "[";
			for(var i:int = 0; i < events_.length; i++) {
				var event:Eventbigcloud = events_[i] as Eventbigcloud;

				result += "{";
				result += '"key":"' + event.key + '",';

				if(event.segmentationKey && event.segmentationValue) {
					result += '"seg_key":"' + event.segmentationKey + '",';
					result += '"seg_val":"' + event.segmentationValue + '",';
				}

				result += '"count":' + event.count + ',';

				if(event.sum) {
					result += '"sum":' + event.sum + ',';
				}

				result += '"timestamp":' + event.timestamp;

				result += "}";

				if(i+1 < events_.length) {
					result += ",";
				}

				event = null;
			}

			events_ = [];

			result += "]";

			return encodeURIComponent(result);
		}

		public function recordEvent(key:String, count:int, sum:Number = 0):void
		{

			for(var i:int = 0; i < events_.length; i++) {
				var event:Eventbigcloud = events_[i] as Eventbigcloud;
				if(event.key == key) {
					event.count += count;
					event.timestamp = bigcloudParse.unixTime();

					if(sum) {
						event.sum += sum;
					}

					events_[i] = event;
					event = null;
					return;
				}
			}

			var newEvent:Eventbigcloud = new Eventbigcloud();
			newEvent.key              = key;
			newEvent.count            = count;
			newEvent.timestamp        = bigcloudParse.unixTime();
			if(sum) {
				newEvent.sum = sum;
			}
			events_.push(newEvent);
			newEvent = null;
		}

		public function recordEventSegmentation(key:String, segmentationKey:String, segmentationValue:String, count:int, sum:Number = 0):void
		{
			for(var i:int = 0; i < events_.length; i++) {
				var event:Eventbigcloud = events_[i] as Eventbigcloud;
				if(event.key == key && event.segmentationKey == segmentationKey && event.segmentationValue == segmentationValue) {
					event.count    += count;
					event.timestamp = bigcloudParse.unixTime();

					if(sum) {
						event.sum += sum;
					}

					events_[i] = event;
					event = null;
					return;
				}
			}

			var newEvent:Eventbigcloud  = new Eventbigcloud();
			newEvent.key               = key;
			newEvent.count             = count;
			newEvent.segmentationKey   = segmentationKey;
			newEvent.segmentationValue = segmentationValue;
			newEvent.timestamp         = BigcloudParse.unixTime();
			if(sum) {
				newEvent.sum = sum;
			}
			events_.push(newEvent);
			newEvent = null;
		}

	}
}
