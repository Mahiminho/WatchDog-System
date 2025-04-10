import { useState } from "react";
import "./App.css";
import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import Statistics from "./components/Statistics/Statistics";

const tabs = ["Main", "Statistics", "Logs", "Translation", "Other"];

export default function App() {
  const [activeTab, setActiveTab] = useState(tabs[0]);

  return (
    <>
      <Header activeTab={activeTab} setActiveTab={setActiveTab} tabs={tabs} />
      {activeTab === "Main" && <Main />}
      {activeTab === "Statistics" && <Statistics />}
    </>
  );
}