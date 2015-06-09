# LinkC Protocol   
## 简述    
LinkC 协议基于JSON（ 没有更多的说明了）    
## 启动连接(Start up connection)    
### 请求
```
"id"        : "id",    
"cmd"       : "connect",
"params"    : {
  "protocol"  : "gurgle",
  "version"   : "unusable",
  "encrypt"   : "disabled"
}
```
现在协议只支持 "gurgle"   
现在版本只存在 "unusable"   
现在加密只支持 "disabled"   
### 回应 (未修改进程序)
```
"id"        : "request's id",
"cmd"       : "kill",    
"reply"     : {
  "status"    : "connection established/failed",
  "error"      :   "short description of error",
  "reason"    :   "human-readable description of reason"
},
```
如果链接正常建立，则只存在"reply"字段中有 "status" : "connection established"
如果链接建立失败，则"reply":{"status":"connection failed","error":"","reason":""},"cmd":"kill"

## 检查延迟(ping)
### 请求
```
"id"        : "id",    
"cmd"       : "ping"    
```
### 回应
```
"id"        : "request's id",    
"reply"     : "pong"
```

## 获取认证方式(Get authentication method)
### 请求
```
"id"        : "id",    
"cmd"       : "query",   
"params"    : {
    "target"        : "auth_method",
    "opinion"       : "list_enabled(default)",
    "replay_style"  : "text(default)"
  }
```
在params中的为可选参数，留空就意味着使用缺省配置    
- target  
  - auth_method    本次请求的为认证方式，故target为auth_method
- opinion     
  - list_enabled   返回服务端目前全部支持的认证方式   
- replay_style      
  - text		       返回文本定义     

### 回应
```
"id"        : "request's id",    
"reply"     : {
  "auth_method"   : ['plain_password']，
  "error"         : null
}
``` 
回复值为一个list

## 获取认证状态(Get authentication status)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "auth_status",
  "error"     : null
}
```
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "auth_status"   : "Unauthenticated/Authenticated",
  "error"         ： null
}
``` 

## 获取协议版本(Get protocol's version)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "version"
}
```
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "version"   : "server's version",
  "error"     : null
}
``` 

## 明文认证(Plain password authentication)
### 请求:
```
"id"        : id,
"cmd"       : "auth", 
"from"      : gurgle_id, 
"params": {
  "method"    : "plain_password",
  "password"  : "password"
  }
}
```
### 回应: [未实现/在程序中修改]
```
"id"  : id, 
"to"  : full_gurgle_id, 
"reply" : {
  "error"   : null,
  "reason"  : null
}
```
to字段严格指明你现在登陆的ID，严格遵守ID的格式。
如果在之前没有指明terminal字段，这里也会为你分配一个
如果登陆失败.to部分也会指明为你之前发送的from字段的数据

## 更新自己状态更新(Publishing self presence update)
### 推送:
```
"id"          : id,
"cmd"         : "push",
"params"      : {
  "target"      : "presence",
  "last_name"   : "new last_name"/None,
  "first_name"  : "new first_name"/None,
  "status"      : "Avaliable/Away/Dnd/Invisible"/None,
  "mood"        : "Your new mood"/None
}
```

### 回应:
```
"id"        : "request's id",    
"reply"     : {
  "error"     : "PermissionDenied"/OtherErrors/null,
  "reason"    : "Unauthenticated"/null
}
```

## 请求自己状态(Get self presence)
### 请求:
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"    : "presence"
}
```
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "target"      : "presence",
  "last_name"   : "last_name"/None,
  "first_name"  : "first_name"/None,
  "status"      : "Avaliable/Away/Dnd/Invisible",
  "mood"        : "Your new mood"/None
  "error"       : null,
  "reason"      : null
}
``` 
None则意味着你想将这个字段置空    
如果不想更改某个字段，可以选择提交原来的状态   
或者不提交这个字段也是可以的
比如：  
```
  "id"        : "request's id",        
  "reply"     : {    
    "target"      : "presence",        
    "last_name"   : "last_name"/None,     
  }    
```
注释：仅修改你的名

## 推送好友列表(Push roster)
### 推送
```
"id"        : 0,
"cmd"       : "push",
"params"    : {
  "target"    : "roster",
  "count"     : 0,1,2,3.......
  "value"     : [
    ["user1","status","mood"],
    ["user2","status","mood"],
    ......
  ]
}
```

## 输出错误(Print errors)
若客户端发来错误格式的报文，予以错误回复
### 回复
```
"id"        : "request's id",
"reply"     : {
  "error"     : "SyntaxError",
  "reason"    : details
  "help"      : "A string to tell you the right syntax"
}
```

## 加入组/添加好友(Friend/group request)
### 请求:
```
"id"        : 0,
"cmd"       : "forward",
"to"        : "A's address",
"params"    : {
  "cmd"       : "request",
  "params"    : {
    "addition"  : "....."
  }
}
```
Id为0表示不需要即时回复

### 转发:
```
"id"        : new id,
"from"      : "original address",
"cmd"       : "request",
"params"    : {
  "addition"  : "....."
}
```

### 回复:
```
"id"        : 0,
"cmd"       : "forward",
"to"        : "original address",
"params"    : {
  "reply"     : {
    "status"  : "accepted/refused",
    "reason"  : "reason",
    "error"   : null
  }
}
```

### 转发:
```
"id"        : 0,
"from"      : "A's address"
"reply"     : {
  "status"    : "accepted/refused",
  "reason"    : "reason",
  "error"     : null
}
```