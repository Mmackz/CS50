const container = document.getElementById("notes")
const back = document.getElementById("folder-back")

function loadFolders() {
    if (!back.classList.contains("d-none")) {
        back.classList.add("d-none")
    }
    chrome.storage.local.get("folders", data => {
        const folders = data.folders
        folders.sort((a, b) => {
            return a.toLowerCase().localeCompare(b.toLowerCase())
        })
        folders.forEach(folder => {
            const newFolder = document.createElement("div")
            newFolder.classList.add("folder")
            newFolder.innerHTML = `
                <img src="https://img.icons8.com/color/48/000000/folder-invoices--v1.png"/>
                <p>${folder}</p>
            `
            newFolder.addEventListener("click", () => {
                loadNotes(folder)
            })
            container.appendChild(newFolder)
        })
    })
}

function loadNotes(folder) {
    if (back.classList.contains("d-none")) {
        back.classList.remove("d-none")
    }
    chrome.storage.local.get(["notes", "folders"], data => {
        let notes = data.notes.filter(note => note.folder === folder)
        console.log(notes.length + " " + "type:" + typeof notes.length)
        if (notes.length === 0) {
            let folders = data.folders
            folders = folders.filter(item => item !== folder || item === "miscellaneous")
            chrome.storage.local.set({folders}, () => {
                container.innerHTML = ""
                if (folder !== "miscellaneous") {
                    loadFolders()
                } else {
                    container.innerHTML = `<p class="mt-4 mb-4">There is no notes here...</p>`
                }
            })
        } else {
            notes = notes.map(note => `<div id=${note.noteId} class='note mb-3'>
                                                                            <i class="remove bi bi-x"></i>
                                                                            <h3 class="note-title">${note.title}</h3>
                                                                            <hr class="note-divider">
                                                                            <span class="note-date">${note.date}</span>
                                                                            <p class="note-body">${note.note}</p>
                                                                            <p class="mb-0">Link: <a class="note-link" href='${note.url}' target='_blank'>${note.url.split("/")[2]}</a></p>
                                                                          </div>
                                                                        `)
            container.innerHTML = notes.join("")
            const removeEls = document.querySelectorAll(".remove")
            removeEls.forEach(el => el.addEventListener("click", event => {
                const elId = event.target.parentNode.id
                const newNotes = data.notes.filter(note => note.noteId !== parseInt(elId))
                chrome.storage.local.set({notes: newNotes}, () => {
                    loadNotes(folder)
                })
            }))
        }
    })
}

loadFolders()