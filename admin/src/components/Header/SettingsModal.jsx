import styles from "./Header.module.css";

const SWITCHES = [
  { key: "fire", label: "Fire detection" },
  { key: "gas", label: "Gas detection" },
  { key: "human", label: "Human detection" },
  { key: "damage", label: "Damage detection" },
];

export default function SettingsModal({
  switches,
  setSwitches,
  onClose,
  onClear,
}) {
  return (
    <div className={styles.settingsModalBackdrop} onClick={onClose}>
      <div
        className={styles.settingsModal}
        onClick={(e) => e.stopPropagation()}
      >
        {SWITCHES.map((sw) => (
          <div key={sw.key} className={styles.switchRow}>
            <label className={styles.switchLabel}>
              <span className={styles.switchIcon}>
                <span
                  className={`${styles.switchEye} ${
                    switches[sw.key] ? styles.switchOn : styles.switchOff
                  }`}
                  onClick={() =>
                    setSwitches((s) => ({ ...s, [sw.key]: !s[sw.key] }))
                  }
                  tabIndex={0}
                  role="button"
                  aria-pressed={switches[sw.key]}
                />
              </span>
              {sw.label}
            </label>
          </div>
        ))}
        <button className={styles.clearStorageBtn} onClick={onClear}>
          Clear local storage
        </button>
      </div>
    </div>
  );
}
