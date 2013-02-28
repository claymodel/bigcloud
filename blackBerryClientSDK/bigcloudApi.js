var bigcloud = {
	serverURL : "",
	app_key : "",
	device_id : null,
	sdk_version : "1.0",
	loop_size : 10,
	metrics : {
		"_os" : "BlackBerry",
		"_os_version" : null,
		"_device" : null,
		"_resolution" : window.screen.width+"x"+window.screen.height,
		"_carrier" : null,
		"_app_version" : null
	},
	beginRequestTime : null,
	lastRequestTime : null,
	timer : null,
	session_duration : 30000,
	events : {},
	events_arr : new Array(),

	setURL : function(url) {
		this.serverURL = url;
	},

	setAppKey : function(key) {
		this.app_key = key;
	},

	init : function() {
		
		bigcloud.device_id = blackberry.identity.uuid;
		bigcloud.metrics._os_version = blackberry.system.softwareVersion;
		bigcloud.metrics._device =  blackberry.system.model;
		bigcloud.metrics._app_version = blackberry.app.version;
		//bigcloud.metrics._resolution = blackberry.screen.width+"x"+blackberry.screen.height;

		this.beginSession();
		blackberry.event.addEventListener("pause", this.endSession);
		blackberry.event.addEventListener("resume", this.beginSession );
		window.addEventListener("unload", function(e) {
         
      }, false);

		//sblackberry.app.event.onForeground(bigcloud.beginSession);
		//blackberry.app.event.onBackground(this.endSession);
		//blackberry.app.event.onExit(this.endSession);
	},

	beginSession : function() {
		console.log("beginSession");
		requestTime = Math.round((new Date()).getTime() / 1000);
		this.beginRequestTime = requestTime;
		this.lastRequestTime  = requestTime;
		bigcloud.metrics._carrier = bigcloud.findCarrier();
		bigcloud.startTimer();
		data = {"app_key" : bigcloud.app_key, "device_id" : bigcloud.device_id, "sdk_version" : bigcloud.sdk_version, "begin_session" : 1, "timestamp" : requestTime, "metrics" : this.metrics};
		bigcloud.push(data);
	},
	
	eventSender : function(key, sum, segment) {
		if (this.events[key]) {
			this.events[key].count += 1;
		}
		else {
			this.events[key] = {key : key, "count" : 1};
		}

		if(typeof(sum) != "undefined" && typeof(sum) == "number") {
			if (this.events[key].hasOwnProperty('sum')) {
				this.events[key].sum += sum;
			}
			else {
				this.events[key].sum = sum;
			}
		}

		if(typeof(segment) != "undefined" && typeof(segment) == "object") {
			if (!this.events[key].segmentation) {
				this.events[key].segmentation = {};
			}
			for(var seg in segment) {
				this.events[key].segmentation[seg] = segment[seg];
			}
		}
		for(evnt in this.events) {
			this.events_arr.push(this.events[evnt]);
		}
	},
	
	schedule : function() {
		if (bigcloud.events_arr.length >= bigcloud.loop_size) {
			request_uri = bigcloud.serverURL+"/i?app_key="+bigcloud.app_key+"&device_id="+bigcloud.device_id+"&session_duration="+bigcloud.session_duration+"&events="+JSON.stringify(bigcloud.events_arr);
			bigcloud.request(request_uri);
			console.log(request_uri);
		}
	},
	
	endSession : function() {
		data = "app_key=" + bigcloud.app_key + "&device_id=" + bigcloud.device_id + "&end_session=1";
		bigcloud.endTimer();
		getreq = bigcloud.serverURL+'/i?'+data;
		bigcloud.request(getreq);
	},
	
	push : function(data) {
		query_string = "";
		query_string2 = "";
		first_segment = 0;
		for (var key in data){
			if (first_segment > 0) {
				query_string += "&";
			}
			if (typeof(data[key]) == "object") {
				query_string2 += JSON.stringify(data[key]);
			}
			else {
				query_string += key+"="+data[key];
			}
			first_segment++;
		}
		query_string2 = escape(query_string2);
		getreq = this.serverURL+'/i?'+query_string+"metrics="+query_string2;
		this.request(getreq);
	},
	
	request : function(url) {
		$.ajax({
			url: url,
			dataType: 'jsonp'
		});
	},
	
	startTimer : function() {
		this.timer = setInterval(this.schedule, this.session_duration);
	},
	
	endTimer : function() {
		clearInterval(this.timer);
	},

	findCarrier : function() {
		// operator = blackberry.identity.IMSI.substring(0,5);
		// if (operator == '28601')
		// {
		// 	return 'Turkcell';
		// }
		// else if (operator == '28602')
		// {
		// 	return 'Vodafone';
		// }
		// else if (operator == '28603')
		// {
		// 	return 'Avea';
		// }
		// else if (operator == '31015')
		// {
		// 	return 'Ripple Operator';
		// }
		// else
		// {
		// 	return 'Unknown Operator';
		// }

		return "Unknow Operator";
	}
};