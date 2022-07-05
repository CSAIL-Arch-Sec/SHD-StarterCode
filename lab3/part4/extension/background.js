const domains = [
  "https://www.google.com/",
  "https://www.youtube.com/",
  "https://www.tmall.com/",
  "https://www.qq.com/",
  "https://www.baidu.com/",
  "https://www.sohu.com/",
  "https://www.facebook.com/",
  "https://www.taobao.com/",
  "https://www.jd.com/",
  "https://www.amazon.com/",
  "https://www.yahoo.com/",
  "https://www.wikipedia.org/",
  "https://www.weibo.com/",
  "https://sina.com.cn/",
  "https://www.zoom.us/",
  "http://www.xinhuanet.com/",
  "https://www.live.com/",
  "https://www.reddit.com/",
  "https://www.netflix.com/",
  "https://www.microsoft.com/",
  "https://www.instagram.com/",
  "https://www.office.com/",
  "https://panda.tv/",
  "https://www.zhanqi.tv/",
  "https://www.alipay.com/",
  "https://www.bing.com/",
  "https://www.csdn.net/",
  "https://www.vk.com/",
  "https://www.myshopify.com/",
  "https://www.naver.com/",
  "https://www.okezone.com/",
  "https://www.twitch.tv/",
  "https://www.twitter.com/",
  "https://www.ebay.com/",
  "https://www.adobe.com/",
  "https://www.tianya.cn/",
  "https://www.huanqiu.com/",
  "https://www.yy.com/",
  "https://www.aliexpress.com/",
  "https://www.linkedin.com/",
  "https://www.force.com/",
  "https://www.aparat.com/",
  "https://www.mail.ru/",
  "https://www.msn.com/",
  "https://www.dropbox.com/",
  "https://www.whatsapp.com/",
  "https://www.apple.com/",
  "https://www.1688.com/",
  "https://www.wordpress.com/",
  "https://www.canva.com/",
  "https://www.indeed.com/",
  "https://www.stackoverflow.com/",
  "https://www.ok.ru/",
  "https://www.so.com/",
  "https://www.chase.com/",
  "https://www.imdb.com/",
  "https://www.slack.com/",
  "https://www.etsy.com/",
  "https://www.tiktok.com/",
  "https://www.booking.com/",
  "https://www.babytree.com/",
  "https://rakuten.co.jp/",
  "https://www.salesforce.com/",
  "https://www.spotify.com/",
  "https://www.tribunnews.com/",
  "https://www.fandom.com/",
  "https://www.tradingview.com/",
  "https://www.github.com/",
  "https://www.haosou.com/",
  "https://www.paypal.com/",
  "https://www.cnblogs.com/",
  "https://www.alibaba.com/",
  "https://www.kompas.com/",
  "https://gome.com.cn/",
  "https://www.walmart.com/",
  "https://www.roblox.com/",
  "https://www.6.cn/",
  "https://www.zillow.com/",
  "https://www.godaddy.com/",
  "https://www.imgur.com/",
  "https://www.espn.com/",
  "https://www.bbc.com/",
  "https://www.hao123.com/",
  "https://www.pikiran-rakyat.com/",
  "https://www.grammarly.com/",
  "https://www.cnn.com/",
  "https://www.telegram.org/",
  "https://www.tumblr.com/",
  "https://www.nytimes.com/",
  "https://www.detik.com/",
  "https://www.wetransfer.com/",
  "https://www.savefrom.net/",
  "https://www.rednet.cn/",
  "https://www.freepik.com/",
  "https://www.ilovepdf.com/",
  "https://www.daum.net/",
  "https://www.pinterest.com/",
  "https://www.primevideo.com/",
  "https://www.intuit.com/",
  "https://www.medium.com/",
];

const loadTime = 5000;
let loading = false;
let startTime = 0;

function randomPing() {
  const controller = new AbortController();
  const id = setTimeout(
    () => controller.abort(),
    loadTime - (performance.now() - startTime)
  );

  fetch(
    domains[Math.floor(Math.random() * domains.length)] +
      "?" +
      new Date().getTime(),
    {
      signal: controller.signal,
    }
  );
}

function activityBurst() {
  switch (Math.floor(Math.random() * 2)) {
    case 0:
      let start = performance.now();
      let counter = 0;

      while (performance.now() - start < 5) {
        counter += 1;
      }

      console.log(counter);
      break;
    case 1:
      randomPing();
      break;
  }
}

chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  if (changeInfo.status === "loading") {
    if (loading) {
      return;
    }

    startTime = performance.now();
    loading = true;

    for (let i = 0; i < 20; i++) {
      randomPing();
    }

    for (let i = 0; i < loadTime / 10; i++) {
      setTimeout(activityBurst, Math.random() * loadTime);
    }
  } else if (changeInfo.status === "complete") {
    loading = false;
  }
});
