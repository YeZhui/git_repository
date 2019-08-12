import { BMapWX } from "./libs/bmap-wx";

//app.js
App({
  globalData: {
    openid:'',
    wxCode:''
  },
  onLaunch: function () {
    var that = this;
    wx.login({
      success (res) {
        if (res.code) {
          that.globalData.wxCode = res.code,
          wx.getUserInfo({
            withCredentials: true,
            success: function(res) {
              wx.request({
                url:'http://192.168.233.250:8000/login',
                data: {
                  code: that.globalData.wxCode,
                  nickName: res.userInfo.nickName
                },
                success: function(res) {
                  that.globalData.openid = res.data.msgInfo,
                  console.log('服务器请求成功' + that.globalData.openid)
                },
                fail: function() {
                  console.log('服务器请求失败')
                },
              })
            } 
          })
        } else {
          console.log('登陆失败' + res.errMsg)
        }
      }
    })
  },
})
