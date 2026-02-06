let currentSlide = 0;
const slides = document.querySelectorAll('.slide');
const totalSlides = slides.length;
const progressContainer = document.getElementById('progress');
const slideContentWrapper = document.querySelector('.slide-content-wrapper');

// Initialize progress dots
for (let i = 0; i < totalSlides; i++) {
    const dot = document.createElement('div');
    dot.className = 'progress-dot';
    if (i === 0) dot.classList.add('active');
    progressContainer.appendChild(dot);
}

const progressDots = document.querySelectorAll('.progress-dot');

// Update slide number
document.querySelector('.total').textContent = totalSlides;

function showSlide(n) {
    // Scroll to top
    slideContentWrapper.scrollTop = 0;
    
    slides[currentSlide].classList.remove('active');
    progressDots[currentSlide].classList.remove('active');
    
    currentSlide = (n + totalSlides) % totalSlides;
    
    slides[currentSlide].classList.add('active');
    progressDots[currentSlide].classList.add('active');
    document.querySelector('.current').textContent = currentSlide + 1;
}

function nextSlide() {
    showSlide(currentSlide + 1);
}

function prevSlide() {
    showSlide(currentSlide - 1);
}

// Keyboard navigation
document.addEventListener('keydown', (e) => {
    if (e.key === 'ArrowRight' || e.key === ' ') {
        e.preventDefault();
        nextSlide();
    } else if (e.key === 'ArrowLeft') {
        e.preventDefault();
        prevSlide();
    } else if (e.key === 'Home') {
        e.preventDefault();
        showSlide(0);
    } else if (e.key === 'End') {
        e.preventDefault();
        showSlide(totalSlides - 1);
    }
});

// Click navigation on the sides
let startX = 0;
slideContentWrapper.addEventListener('mousedown', (e) => {
    startX = e.clientX;
});

slideContentWrapper.addEventListener('mouseup', (e) => {
    const endX = e.clientX;
    const diff = startX - endX;
    
    // Only navigate if it was a click (not a drag)
    if (Math.abs(diff) < 5) {
        if (e.clientX > window.innerWidth / 2) {
            nextSlide();
        } else {
            prevSlide();
        }
    }
});

// Touch support
let touchStartX = 0;
slideContentWrapper.addEventListener('touchstart', (e) => {
    touchStartX = e.touches[0].clientX;
});

slideContentWrapper.addEventListener('touchend', (e) => {
    const touchEndX = e.changedTouches[0].clientX;
    const diff = touchStartX - touchEndX;

    if (Math.abs(diff) > 50) {
        if (diff > 0) {
            nextSlide();
        } else {
            prevSlide();
        }
    }
});

// ========================================
// NAVIGATION ENHANCEMENTS
// ========================================

// Klickbare Next-Cards (für "Nächster Schritt" Boxen)
document.querySelectorAll('.card-dark[data-next]').forEach(card => {
    card.addEventListener('click', () => {
        window.location.href = card.dataset.next;
    });
});

// Erweiterte Keyboard-Navigation
document.addEventListener('keydown', (e) => {
    // N = Zum nächsten Modul (nur auf letzter Slide)
    if ((e.key === 'n' || e.key === 'N') && !e.ctrlKey && !e.metaKey) {
        const nextCard = document.querySelector('.card-dark[data-next]');
        if (nextCard && currentSlide === totalSlides - 1) {
            e.preventDefault();
            window.location.href = nextCard.dataset.next;
        }
    }

    // B = Zurück zur Übersicht
    if ((e.key === 'b' || e.key === 'B') && !e.ctrlKey && !e.metaKey) {
        const backLink = document.querySelector('.navigation a');
        if (backLink) {
            e.preventDefault();
            window.location.href = backLink.href;
        }
    }
});
