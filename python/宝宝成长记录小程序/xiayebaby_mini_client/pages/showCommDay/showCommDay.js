// pages/showCommDay/showCommDay.js
var appInstance = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    commDays: [],  //列表里面是字典元素
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    //发起请求，获取纪念日数据
    //点击确定按钮调用
    var that = this
    console.log('开始获取纪念日!')
    wx.request({
      url: 'http://192.168.233.250:8000/show_comm_day', 
      data: {
        custId: appInstance.globalData.openid,
      },
      success (res) {
        console.log(res.data)
        that.data.commDays = res.data.commDays
      },
      fail (res) {
        console.log('获取纪念日失败!')
      }
    })
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})