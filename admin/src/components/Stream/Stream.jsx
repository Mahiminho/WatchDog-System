import { useState, useEffect } from "react";
import styles from "./Stream.module.css";
import { LoadingDots } from "../Main/Main";

export default function Stream({ streamUrl }) {
  const [loading, setLoading] = useState(true);
  const [available, setAvailable] = useState(true);
  const [iframeKey, setIframeKey] = useState(0);

  useEffect(() => {
    if (!streamUrl) return;
    const check = () => {
      fetch(streamUrl, { method: "GET", mode: "no-cors" })
        .then(() => {
          if (!available) setIframeKey((k) => k + 1);
          setAvailable(true);
        })
        .catch(() => {
          setAvailable(false);
          setLoading(true);
        });
    };
    check();
    const interval = setInterval(check, 1000);
    return () => clearInterval(interval);
  }, [streamUrl, available]);

  if (!streamUrl) {
    return (
      <div
        style={{
          width: "100%",
          height: "100%",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          background: "#181818",
        }}
      >
        <LoadingDots />
      </div>
    );
  }

  if (!available) {
    return (
      <div
        style={{
          width: "100%",
          height: "100%",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          background: "#181818",
        }}
      >
        <LoadingDots />
      </div>
    );
  }

  if (loading) {
    return (
      <div
        style={{
          width: "100%",
          height: "100%",
          display: "flex",
          alignItems: "center",
          justifyContent: "center",
          background: "#181818",
        }}
      >
        <LoadingDots />
        <iframe
          key={iframeKey}
          src={streamUrl}
          title="Camera Stream"
          style={{ display: "none" }}
          onLoad={() => setLoading(false)}
        />
      </div>
    );
  }

  return (
    <div className={styles.streamRoot}>
      <iframe
        key={iframeKey}
        src={streamUrl}
        title="Camera Stream"
        className={styles.iframe}
        frameBorder={0}
        allowFullScreen
        onLoad={() => setLoading(false)}
      />
    </div>
  );
}
