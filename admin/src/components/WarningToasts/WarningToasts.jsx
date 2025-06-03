import styles from "./WarningToasts.module.css";

export default function WarningToasts({ warnings, onClose }) {
  return (
    <div className={styles.toastContainer}>
      {warnings.map(
        (w) =>
          w.visible && (
            <div
              key={w.key}
              className={styles.toast}
              tabIndex={0}
              title={w.label}
            >
              <span className={styles.icon}>
                <span className={styles.exMark}>!</span>
              </span>
              <span className={styles.text}>{w.label}</span>
              <button
                className={styles.closeBtn}
                onClick={() => onClose(w.key)}
              >
                ×
              </button>
            </div>
          )
      )}
    </div>
  );
}
