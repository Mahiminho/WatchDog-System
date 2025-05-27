import { useState, useEffect } from "react";
import "./App.css";
import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import Statistics from "./components/Statistics/Statistics";
import Logs from "./components/Logs/Logs";

const tabs = ["Main", "Statistics", "Logs", "Stream", "Data"];
const API_URL = "http://localhost:8000";

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
        .then((json) =>
          setSingleSensorInsights(json?.single_sensor_insights || [])
        )
        .catch(() => setSingleSensorInsights([]));
    };

    const fetchCombinedSensors = () => {
      fetch(`${API_URL}/combined-sensors-results`)
        .then((res) => (res.ok ? res.json() : null))
        .then((json) =>
          setCombinedSensorInsights(json?.combined_sensor_insights || [])
        )
        .catch(() => setCombinedSensorInsights([]));
    };

    fetchData();
    fetchEnvReport();
    fetchSingleSensors();
    fetchCombinedSensors();

    const interval = setInterval(() => {
      fetchData();
      fetchEnvReport();
      fetchSingleSensors();
      fetchCombinedSensors();
    }, 1000);

    return () => clearInterval(interval);
  }, []);

  return (
    <>
      <Header activeTab={activeTab} setActiveTab={setActiveTab} tabs={tabs} />
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
    </>
  );
}
