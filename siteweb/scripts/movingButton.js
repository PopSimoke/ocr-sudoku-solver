let isRunningAway = true;

function runAway(id) {
    if (isRunningAway) {
        id.style.top = (Math.round(Math.random() * 300) - 150) * 2 + 'px';
        id.style.left = (Math.round(Math.random() * 300) - 300) * 2 + 'px';
    }
}

function stopRunAway() {
    isRunningAway = false;
        document.getElementById('startButton').classList.remove('noHover'); // Réactiver :hover
}

// Désactive :hover lorsque le déplacement est actif
function disableHover() {
    if (isRunningAway) {
        document.getElementById('startButton').classList.add('noHover');
    }
}

// Réactive :hover lorsque le déplacement n'est pas actif
function enableHover() {
    if (!isRunningAway) {
        document.getElementById('startButton').classList.remove('noHover');
    }
}

// Applique les fonctions disableHover et enableHover sur le bouton startButton
document.getElementById('startButton').addEventListener('mouseover', disableHover);
document.getElementById('startButton').addEventListener('mouseout', enableHover);