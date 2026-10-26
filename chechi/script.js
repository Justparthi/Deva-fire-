const screens = [...document.querySelectorAll(".screen")];
const progressBar = document.querySelector("#progressBar");
const stepCurrent = document.querySelector("#stepCurrent");
const nextButtons = document.querySelectorAll(".next-button");
const choices = document.querySelectorAll(".choice");
const choiceMessage = document.querySelector("#choiceMessage");
const restartButton = document.querySelector(".restart-button");
const brand = document.querySelector(".brand");
let currentScreen = 0;

function showScreen(index) {
  currentScreen = index;
  screens.forEach((screen, screenIndex) => {
    screen.classList.toggle("active", screenIndex === index);
  });
  stepCurrent.textContent = String(index + 1).padStart(2, "0");
  progressBar.style.width = `${((index + 1) / screens.length) * 100}%`;
  window.scrollTo({ top: 0, behavior: "smooth" });
}

nextButtons.forEach((button) => {
  button.addEventListener("click", () => showScreen(Math.min(currentScreen + 1, screens.length - 1)));
});

choices.forEach((choice) => {
  choice.addEventListener("click", () => {
    choices.forEach((item) => item.classList.remove("selected"));
    choice.classList.add("selected");
    choiceMessage.textContent = choice.dataset.choice === "yes"
      ? "I knew it. I miss you right back. ♡"
      : "That’s okay, I’ll just have to remind you. ♡";
    setTimeout(() => showScreen(2), 650);
  });
});

function restart(event) {
  event.preventDefault();
  choices.forEach((choice) => choice.classList.remove("selected"));
  choiceMessage.textContent = "There is a right answer, obviously.";
  showScreen(0);
}

restartButton.addEventListener("click", restart);
brand.addEventListener("click", restart);
