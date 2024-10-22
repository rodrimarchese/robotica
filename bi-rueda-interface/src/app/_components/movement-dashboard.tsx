"use client";

import React, { useState, useRef, useEffect } from "react";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Card, CardContent } from "@/components/ui/card";
import axios from "axios";

export default function MovementDashboard() {
  const [apiUrl, setApiUrl] = useState("");
  const [savedApiUrl, setSavedApiUrl] = useState("http://172.22.39.98:5000");
  const joystickRef = useRef<HTMLDivElement>(null);
  const containerRef = useRef<HTMLDivElement>(null); // Añadido
  const [joystickPosition, setJoystickPosition] = useState({ x: 0, y: 0 });
  const [currentCommand, setCurrentCommand] = useState<
    keyof typeof commandMap | ""
  >("");

  const commandMap = {
    "65": "Adelante",
    "83": "Atrás",
    "68": "Derecha",
    "69": "Izquierda",
    "70": "Frenar",
  };

  const saveApiUrl = () => {
    let url = apiUrl.trim();
    if (!url.startsWith("http://") && !url.startsWith("https://")) {
      url = "http://" + url;
    }
    setSavedApiUrl(url);
  };

  const handleApiUrlChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setApiUrl(e.target.value);
  };

  const sendCommand = async (command: string) => {
    if (!savedApiUrl) {
      console.error("API URL not set");
      return;
    }

    try {
      const response = await axios.get(`${savedApiUrl}/execute/${command}`);
      console.log(`Command ${command} sent successfully`, response.data);
    } catch (error) {
      console.error("Error sending command:", error);
    }
  };

  useEffect(() => {
    const joystick = joystickRef.current;
    const container = containerRef.current; // Añadido
    if (!joystick || !container) return;

    let isDragging = false;

    const getRelativePosition = (clientX: number, clientY: number) => {
      const rect = container.getBoundingClientRect(); // Usar el contenedor
      const x = clientX - rect.left - rect.width / 2;
      const y = clientY - rect.top - rect.height / 2;
      return { x, y };
    };

    const updateJoystickPosition = (x: number, y: number) => {
      const maxDistance = 50;
      const distance = Math.sqrt(x * x + y * y);
      const angle = Math.atan2(y, x);

      const clampedDistance = Math.min(distance, maxDistance);
      const newX = Math.cos(angle) * clampedDistance;
      const newY = Math.sin(angle) * clampedDistance;

      setJoystickPosition({ x: newX, y: newY });

      // Determinar el comando basado en la posición del joystick
      if (Math.abs(newX) > Math.abs(newY)) {
        if (newX > 0) {
          setCurrentCommand("68"); // Derecha
        } else {
          setCurrentCommand("69"); // Izquierda
        }
      } else {
        if (newY < 0) {
          setCurrentCommand("65"); // Adelante
        } else if (newY > 0) {
          setCurrentCommand("83"); // Atrás
        } else {
          setCurrentCommand("70"); // Parar
        }
      }
    };

    const handleStart = (clientX: number, clientY: number) => {
      isDragging = true;
      const { x, y } = getRelativePosition(clientX, clientY);
      updateJoystickPosition(x, y);
    };

    const handleMove = (clientX: number, clientY: number) => {
      if (!isDragging) return;
      const { x, y } = getRelativePosition(clientX, clientY);
      updateJoystickPosition(x, y);
    };

    const handleEnd = () => {
      isDragging = false;
      setJoystickPosition({ x: 0, y: 0 });
      setCurrentCommand("70"); // Parar cuando se suelta
    };

    const handleMouseDown = (e: MouseEvent) => {
      e.preventDefault();
      handleStart(e.clientX, e.clientY);
    };
    const handleMouseMove = (e: MouseEvent) => handleMove(e.clientX, e.clientY);
    const handleMouseUp = handleEnd;

    const handleTouchStart = (e: TouchEvent) => {
      e.preventDefault();
      const touch = e.touches[0];
      handleStart(touch.clientX, touch.clientY);
    };
    const handleTouchMove = (e: TouchEvent) => {
      e.preventDefault();
      const touch = e.touches[0];
      handleMove(touch.clientX, touch.clientY);
    };
    const handleTouchEnd = (e: TouchEvent) => {
      e.preventDefault();
      handleEnd();
    };

    // Añadir eventos al contenedor
    container.addEventListener("mousedown", handleMouseDown);
    container.addEventListener("touchstart", handleTouchStart);

    document.addEventListener("mousemove", handleMouseMove);
    document.addEventListener("mouseup", handleMouseUp);

    document.addEventListener("touchmove", handleTouchMove);
    document.addEventListener("touchend", handleTouchEnd);

    return () => {
      container.removeEventListener("mousedown", handleMouseDown);
      container.removeEventListener("touchstart", handleTouchStart);

      document.removeEventListener("mousemove", handleMouseMove);
      document.removeEventListener("mouseup", handleMouseUp);

      document.removeEventListener("touchmove", handleTouchMove);
      document.removeEventListener("touchend", handleTouchEnd);
    };
  }, []);

  useEffect(() => {
    if (currentCommand) {
      sendCommand(currentCommand);
    }
  }, [currentCommand, savedApiUrl]);

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100 p-4">
      {/* big tittle "Control Bi-Rueda" */}
      <h1 className="text-4xl font-semibold text-center mb-4">
        Control Bi-Rueda
      </h1>

      <Card className="w-full max-w-md">
        <CardContent className="space-y-4 p-6">
          <div className="flex space-x-2">
            <Input
              type="text"
              value={apiUrl}
              onChange={handleApiUrlChange}
              placeholder="Enter API URL"
              className="flex-grow"
            />
            <Button onClick={saveApiUrl}>Save</Button>
          </div>
          <div className="text-center">
            <p className="text-sm text-gray-500">
              API URL actual: {savedApiUrl || "No especificada"}
            </p>
          </div>
        </CardContent>
      </Card>

      <Card className="w-full max-w-md mt-4">
        <CardContent className="space-y-4 p-6">
          <div
            className="relative w-48 h-48 mx-auto"
            ref={containerRef} // Añadido
          >
            <div className="absolute inset-0 border-2 border-gray-300 rounded-full" />
            <div
              ref={joystickRef}
              className="absolute w-24 h-24 bg-blue-500 rounded-full cursor-move transition-transform duration-75 ease-out"
              style={{
                top: "50%",
                left: "50%",
                transform: `translate(-50%, -50%) translate(${joystickPosition.x}px, ${joystickPosition.y}px)`,
              }}
            >
              <div className="absolute inset-0 flex items-center justify-center text-white font-bold">
                Move
              </div>
            </div>
          </div>
          <div className="text-center">
            <p className="text-sm font-semibold">
              Orden actual:{" "}
              {currentCommand && commandMap[currentCommand]
                ? commandMap[currentCommand]
                : "Frenado"}
            </p>
            <p className="text-sm">Command: {currentCommand}</p>
          </div>
        </CardContent>
      </Card>
    </div>
  );
}
