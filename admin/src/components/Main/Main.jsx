import { useState, useEffect } from "react";
import styles from "./Main.module.css";
import logo from "../../assets/wds-logo.svg";
import air from "../../assets/main-pics/air.svg";
import light from "../../assets/main-pics/light.svg";
import sound from "../../assets/main-pics/sound.svg";
import temp from "../../assets/main-pics/temp.svg";

export default function Main({ data, insights }) {
  const [hovered, setHovered] = useState(null);
  const [tooltipPos, setTooltipPos] = useState({ x: 0, y: 0 });

  const cards = [
    {
      icon: temp,
      alt: "Temperature",
      label: "Temperature",
      value: data?.tempData,
      unit: "°C",
      insight: insights[0] || "",
    },
    {
      icon: light,
      alt: "Light",
      label: "Luminosity",
      value: data?.lightData,
      unit: "Lux",
      insight: insights[2] || "",
    },
    {
      icon: air,
      alt: "Air Quality",
      label: "Air quality",
      value: data?.airData,
      unit: "PPM",
      insight: insights[1] || "",
    },
    {
      icon: sound,
      alt: "Sound",
      label: "Sound",
      value: data?.soundData,
      unit: "dB",
      insight: insights[3] || "",
    },
  ];

  const handleMouseMove = (e) => {
    setTooltipPos({ x: e.clientX, y: e.clientY });
  };

  return (
    <>
      {data ? (
        <div className={styles.container}>
          {cards.map((card, idx) => (
            <div
              key={card.label}
              className={styles.card}
              onMouseEnter={() => setHovered(idx)}
              onMouseLeave={() => setHovered(null)}
              onMouseMove={handleMouseMove}
              style={{ position: "relative" }}
            >
              <img src={card.icon} alt={card.alt} className={styles.image} />
              <span className={styles.text}>
                {card.label}: {card.value ?? "N/A"} {card.unit}
              </span>
              {hovered === idx && card.insight && (
                <div
                  className={styles.tooltip}
                  style={{
                    left: tooltipPos.x,
                    top: tooltipPos.y,
                    position: "fixed",
                    transform: "translate(10px, 10px)",
                    pointerEvents: "none",
                  }}
                >
                  {card.insight}
                </div>
              )}
            </div>
          ))}
        </div>
      ) : (
        <LoadingDots />
      )}
    </>
  );
}

export function LoadingDots() {
  const [dots, setDots] = useState(1);

  useEffect(() => {
    const interval = setInterval(() => {
      setDots((prev) => (prev === 3 ? 1 : prev + 1));
    }, 500);
    return () => clearInterval(interval);
  }, []);

  return (
    <div className={styles.loading}>
      <img src={logo} alt="Logo" className={styles.loadingLogo} />
      <span className={styles.loadingText}>Loading{".".repeat(dots)}</span>
    </div>
  );
}
