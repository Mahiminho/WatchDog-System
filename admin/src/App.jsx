import { useState, useEffect } from "react";
import "./App.css";
import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import Statistics from "./components/Statistics/Statistics";
import Logs from "./components/Logs/Logs";
import Stream from "./components/Stream/Stream";
import Data from "./components/Data/Data";
import SettingsModal from "./components/Header/SettingsModal";
import WarningToasts from "./components/WarningToasts/WarningToasts";

const tabs = ["Main", "Statistics", "Logs", "Stream", "Data"];
const API_URL = "http://localhost:8000";
const WARNING_TYPES = [
  { key: "fire", label: "Detected fire!", scoreKey: "fireScore" },
  { key: "gas", label: "Detected gas!", scoreKey: "gasScore" },
  { key: "human", label: "Detected human!", scoreKey: "humanScore" },
  { key: "damage", label: "Detected damage!", scoreKey: "damageScore" },
];
const WARNING_THRESHOLD = 70; // percentage threshold for warnings

export default function App() {
  const [activeTab, setActiveTab] = useState(tabs[0]);
  const [data, setData] = useState(null);
  const [insights, setInsights] = useState([]);
  const [history, setHistory] = useState([]);
  const [historyLoading, setHistoryLoading] = useState(true);
  const [envReport, setEnvReport] = useState(null);
  const [singleSensorInsights, setSingleSensorInsights] = useState([]);
  const [combinedSensorInsights, setCombinedSensorInsights] = useState([]);
  const [envReportTimestamp, setEnvReportTimestamp] = useState(null);
  const [streamUrl, setStreamUrl] = useState(null);
  const [showSettings, setShowSettings] = useState(false);
  const [switches, setSwitches] = useState({
    fire: false,
    gas: false,
    human: false,
    damage: false,
  });
  const [sensorScores, setSensorScores] = useState({});
  const [warningBlocks, setWarningBlocks] = useState({
    fire: { visible: false, blockedUntil: 0 },
    gas: { visible: false, blockedUntil: 0 },
    human: { visible: false, blockedUntil: 0 },
    damage: { visible: false, blockedUntil: 0 },
  });

  useEffect(() => {
    const savedTab = localStorage.getItem("activeTab");
    if (savedTab && tabs.includes(savedTab)) {
      setActiveTab(savedTab);
    }
  }, []);

  useEffect(() => {
    localStorage.setItem("activeTab", activeTab);
  }, [activeTab]);

  useEffect(() => {
    setHistoryLoading(true);
    const saved = localStorage.getItem("sensorHistory");
    if (saved) {
      try {
        const parsed = JSON.parse(saved).map((d) => ({
          ...d,
          time: new Date(d.time),
        }));
        setHistory(
          parsed.filter((d) => new Date() - new Date(d.time) <= 300000)
        );
      } catch {
        setHistory([]);
      }
    }
    setHistoryLoading(false);
  }, []);

  useEffect(() => {
    if (history.length) {
      localStorage.setItem("sensorHistory", JSON.stringify(history));
    }
  }, [history]);

  useEffect(() => {
    let interval = setInterval(() => {
      fetch(`${API_URL}/sensors-data`)
        .then((r) => r.json())
        .then((data) => setSensorScores(data))
        .catch(() => {});
    }, 2000);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    setWarningBlocks((prev) => {
      const now = Date.now();
      let updated = { ...prev };
      for (const type of WARNING_TYPES) {
        const swOn = switches[type.key];
        const score = sensorScores[type.scoreKey] ?? 0;
        const blocked = prev[type.key].blockedUntil > now;
        if (swOn && score >= WARNING_THRESHOLD && !blocked) {
          updated[type.key] = { ...updated[type.key], visible: true };
        } else {
          updated[type.key] = { ...updated[type.key], visible: false };
        }
        if (!swOn) {
          updated[type.key] = { ...updated[type.key], visible: false };
        }
      }
      return updated;
    });
  }, [sensorScores, switches]);

  const handleCloseWarning = (key) => {
    setWarningBlocks((prev) => ({
      ...prev,
      [key]: { ...prev[key], visible: false, blockedUntil: Date.now() + 30000 },
    }));
  };

  const warnings = WARNING_TYPES.map((type) => ({
    key: type.key,
    label: type.label,
    visible: warningBlocks[type.key].visible,
  }));

  useEffect(() => {
    const fetchData = () => {
      fetch(`${API_URL}/sensors-data`)
        .then((response) => {
          if (!response.ok) throw new Error("No data");
          return response.json();
        })
        .then((json) => {
          if (
            typeof json.temperature === "number" &&
            typeof json.airPPM === "number" &&
            typeof json.lightLux === "number" &&
            typeof json.noiseDB === "number"
          ) {
            const newData = {
              time: new Date(),
              tempData: json.temperature,
              airData: json.airPPM,
              lightData: json.lightLux,
              soundData: json.noiseDB,
            };
            setData(newData);

            setHistory((prev) => {
              const updated = [...prev, newData].filter(
                (d) => new Date() - d.time <= 300000
              );
              return updated;
            });
            setHistoryLoading(false);
          } else {
            setData(null);
            setHistoryLoading(false);
          }
        })
        .catch(() => {
          setData(null);
          setHistory([]);
          setHistoryLoading(false);
        });
    };

    const fetchEnvReport = () => {
      fetch(`${API_URL}/enviroment-report`)
        .then((res) => (res.ok ? res.json() : null))
        .then((json) => {
          setEnvReport(json?.ai_report || null);
          setEnvReportTimestamp(json?.timestamp || null);
        })
        .catch(() => setEnvReport(null));
    };

    const fetchSingleSensors = () => {
      fetch(`${API_URL}/single-sensors-results`)
        .then((res) => (res.ok ? res.json() : null))
        .then((json) => {
          const arr = json?.single_sensor_insights || [];
          setSingleSensorInsights(arr);
          setInsights(arr);
        })
        .catch(() => {
          setSingleSensorInsights([]);
          setInsights([]);
        });
    };

    const fetchCombinedSensors = () => {
      fetch(`${API_URL}/combined-sensors-results`)
        .then((res) => (res.ok ? res.json() : null))
        .then((json) =>
          setCombinedSensorInsights(json?.combined_sensor_insights || [])
        )
        .catch(() => setCombinedSensorInsights([]));
    };

    const fetchStreamUrl = () => {
      fetch(`${API_URL}/stream-url`)
        .then((res) => (res.ok ? res.json() : null))
        .then((json) => setStreamUrl(json?.url || null))
        .catch(() => setStreamUrl(null));
    };

    fetchData();
    fetchEnvReport();
    fetchSingleSensors();
    fetchCombinedSensors();
    fetchStreamUrl();

    const interval = setInterval(() => {
      fetchData();
      fetchEnvReport();
      fetchSingleSensors();
      fetchCombinedSensors();
      fetchStreamUrl();
    }, 1000);

    return () => clearInterval(interval);
  }, []);

  return (
    <>
      <Header
        activeTab={activeTab}
        setActiveTab={setActiveTab}
        tabs={tabs}
        onShowSettings={() => setShowSettings(true)}
      />
      {showSettings && (
        <SettingsModal
          switches={switches}
          setSwitches={setSwitches}
          onClose={() => setShowSettings(false)}
          onClear={() => {
            localStorage.clear();
            window.location.reload();
          }}
        />
      )}
      {activeTab === "Main" && <Main data={data} insights={insights} />}
      {activeTab === "Statistics" && (
        <Statistics history={history} loading={historyLoading} />
      )}
      {activeTab === "Logs" && (
        <Logs
          envReport={envReport}
          envReportTimestamp={envReportTimestamp}
          singleSensorInsights={singleSensorInsights}
          combinedSensorInsights={combinedSensorInsights}
        />
      )}
      {activeTab === "Stream" && <Stream streamUrl={streamUrl} />}
      {activeTab === "Data" && <Data API_URL={API_URL} />}
      <WarningToasts warnings={warnings} onClose={handleCloseWarning} />
    </>
  );
}
