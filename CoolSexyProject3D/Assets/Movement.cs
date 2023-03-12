using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;

public class Movement : MonoBehaviour {
  
  SerialPort stream = new SerialPort("/dev/cu.usbmodem14301", 9600);

  float pitch = 0;
  float roll = 0;
  float yaw = 0;
  float accX = 0;
  float accY = 0;
  float accZ = 0;
  Vector3 motion;
  string data;
  int distance;
  Rigidbody rb;


  void Start() {
        stream.Open();
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update() {
        data = stream.ReadLine();
        string[] values = data.Split(",");
        try {
            accX = 2*float.Parse(values[1]);
            accY = 2*float.Parse(values[2]);
            accZ = 2*float.Parse(values[3]);
            roll = float.Parse(values[4]);
            pitch = float.Parse(values[5]);
            yaw = float.Parse(values[6]);
            distance = int.Parse(values[7]);
        }
        catch {
            pitch = 0;
            roll = 0;
            yaw = 0;
            accX = 0;
            accY = 0;
            accZ = 0;
        }

        motion = new Vector3((int) (accX), (int) (accY), (int) (accZ));
        rb.velocity = motion*2;

    }

    void OnGUI() {
        GUI.Label(new Rect(10,10,300,100), data);
    }
}