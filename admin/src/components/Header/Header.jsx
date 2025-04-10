import wdsLogo from "../../assets/wds-logo.svg";
import styles from "./Header.module.css";

export default function Header({ activeTab, setActiveTab, tabs }) {
  return (
    <>
      <header className={styles.header}>
        <img src={wdsLogo} alt="Logo" className={styles.logoContainer} />
        <nav className={styles.nav}>
          {tabs.map((tab) => (
            <button
              key={tab}
              className={`${styles.tab} ${
                activeTab === tab ? styles.active : ""
              }`}
              onClick={() => setActiveTab(tab)}
            >
              {tab}
            </button>
          ))}
        </nav>
        <button className={styles.helpButton}>?</button>
      </header>
    </>
  );
}
