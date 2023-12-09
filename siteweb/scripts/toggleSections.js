function toggleSections() {
    // Ajoutez ici le code pour basculer entre les sections
    var startButton = document.getElementById("startButton");
    var stopButton = document.getElementById("stopButton");
    var currentSection = document.getElementById("section1");
    var nextSection = document.getElementById("section2");

    // Masquer la section actuelle
    currentSection.classList.add("hidden");
    startButton.classList.add("disabled");
    stopButton.classList.add("disabled");
    startButton.classList.add("hidden");
    stopButton.classList.add("hidden");


    // Afficher la nouvelle section
    nextSection.classList.remove("hidden");
    
    // Appliquer la transition (par exemple, changer la couleur de fond)
    document.body.style.backgroundColor = "#f0f0f0"; // Changez la couleur selon vos besoins
}