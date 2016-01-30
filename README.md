The bigcloud framework consists of two spaces

1. Backend Space
2. Clientend Space
3. Parse Backend 


```
curl -X POST \
  -H "X-Parse-Application-Id: myAppId" \
  -H "Content-Type: application/json" \
  -d '{"score":1337,"playerName":"Sean Plott","cheatMode":false}' \
  http://localhost:1337/parse/classes/GameScore
  
curl -X POST \
  -H "X-Parse-Application-Id: myAppId" \
  -H "Content-Type: application/json" \
  -d '{}' \
  http://localhost:1337/parse/functions/hello
```

Backend Space leverages Ruby based Web App and Web Admin mainly. There are also mongoDB,couchDB 
backend for  aggregating the analytics and user activity logs from Hadoop based middleware. This 
middleware uses machine learning tools (mahout, jubatus). The Backend web app is modulerized so 
that its easier to integrate the facebook graph, gcm console, kakao people, nhn sdk, gree sdk, dena sdk apis.

Clientend consists of Android, Blackberry, iOS, Bada client, Web integration SDKs.


MIT License
