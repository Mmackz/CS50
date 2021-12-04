const newNote = document.getElementById("new-note")
const reset = document.getElementById("reset")
const formContainer = document.getElementById("form-container")
const btn = document.getElementById("btn")
const selectEl = document.getElementById("select")
const myFolders = document.getElementById("folders")
const newFolder = document.getElementById("new-folder")
const addFolder = document.getElementById("add-folder")
const backBtn = document.getElementById("back-btn")

async function getCurrentTab() {
    let queryOptions = { active: true, currentWindow: true };
    let [tab] = await chrome.tabs.query(queryOptions);
    return tab.url;
  }

newNote.addEventListener("submit", async event => {
    event.preventDefault()
    const title = newNote.elements["title"].value
    const note = newNote.elements["note"].value
    const folder = newNote.elements["new-folder"].value || "miscellaneous"

    const url = await getCurrentTab()
    chrome.storage.local.get(["notes", "folders"], data => {
        const prevNotes = data.notes
        const prevFolders = data.folders
        let date = new Date()
        const noteId = Date.now()
        date = date.toLocaleString('en-US', { timeZone: 'PST', dateStyle: 'medium', timeStyle: 'short' })
        if (!prevFolders.includes(folder)) {
            chrome.storage.local.set({folders: [folder, ...prevFolders]})
        }
        chrome.storage.local.set({notes: [{title, note, url, folder, date, noteId}, ...prevNotes]}, () => {
            newNote.elements["title"].value = ""
            newNote.elements["note"].value = ""
            newFolder.value = ""
            renderForm(true, folder)
        })
    })

    if (selectEl.classList.contains("d-none")) {
        selectEl.classList.remove("d-none")
        addFolder.classList.remove("d-none")
        newFolder.classList.add("d-none")
        backBtn.classList.add("d-none")
    }
})

reset.addEventListener("click", () => {
    chrome.storage.local.clear(() => {
        chrome.storage.local.set({notes: []})
        chrome.storage.local.set({folders: ["miscellaneous"]})
        renderForm()
    })
    
})

addFolder.addEventListener("click", () => {
    selectEl.classList.add("d-none")
    addFolder.classList.add("d-none")
    newFolder.classList.remove("d-none")
    backBtn.classList.remove("d-none")
    newFolder.value = ""
    newFolder.focus()
})

addFolder.addEventListener("mouseenter", () => {
    document.getElementById("folder-btn").focus()
})

addFolder.addEventListener("mouseleave", () => {
    document.getElementById("folder-btn").blur()
})

backBtn.addEventListener("click", () => {
    selectEl.classList.remove("d-none")
    addFolder.classList.remove("d-none")
    newFolder.classList.add("d-none")
    backBtn.classList.add("d-none")
})

myFolders.addEventListener("change", event => {
    newFolder.value = event.target.value
})


function renderForm(bool = false, folder = null) {
    if (bool) {
        const added = document.getElementById("added")
        if (added.classList.contains("d-none")) {
            added.classList.remove("d-none")
            setTimeout(() => {
                added.classList.add("d-none")
            }, 3000)
        }
    }

    chrome.storage.local.get("folders", data => {
        let foldersToRender = data.folders.filter(folder => folder !== "miscellaneous")
        foldersToRender = foldersToRender.map(folder => `<option>${folder}</option>`)
        myFolders.innerHTML = `<option disabled>--please select a folder--</option>
                               <option value="miscellaneous">miscellaneous</option>
                               ${foldersToRender.join('')}`
        myFolders.value = folder || "miscellaneous"
        newFolder.value = myFolders.value
    })
}

renderForm()