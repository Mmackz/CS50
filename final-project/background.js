chrome.runtime.onInstalled.addListener(function() {
    chrome.storage.local.set({notes: []})
    chrome.storage.local.set({folders: ["miscellaneous"]})
  })