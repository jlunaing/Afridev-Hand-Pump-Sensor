# Monitoring System for the Afridev Hand Pump

## Overview

[Source code](src), [circuit schematic and board files](pcb), and [**technical report**](docs) of a proof-of-concept electronic system to monitor the status of the Afridev model of hand pumps.

<p align="middle">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/img/prototype_02.png" 
   width=60%    
   alt="Figure 1"/>
</p>

<p align="middle">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/img/v2/assembly_01.png" 
   width=60%    
   alt="Figure 2"/>
</p>

<p align="middle">
  <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/img/afridev_sensor_01.png" width="100" alt="Figure 3A"/>
  <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/img/afridev_sensor_02.png" width="100" alt="Figure 3B"/> 
</p>

## Introduction

Different companies, non-profit organizations, and NGOs have implemented water projects in several regions of Africa. One of the most common projects are hand pumps for water wells. A challenge that is presented once these projects are concluded and used by the local residents are that there is no regular maintenance of these hand pumps due to communication difficulties with the local communities. For instance, in most of the Democratic Republic of the Congo (DRC), the lack of cellular signal and internet connection makes it more difficult for repair teams to know when the hand pumps need repair or maintenance. 

The objective of this project is to propose a system that can reduce the time between pump failure and repair by developing a reliable failure detection measuring system that will inform a user when a pump need attention from a repair team. The system will collect data on pump usage and water flow rate estimation to be accessed remotely.

The reduction in hand pump repair time would increase the consistency of well water available to people in the remote areas like the DRC.

## Technical Background

Currently, there are no commercially available options for well pump sensors with the capabilities to detect pump function, monitor water flow rate, and transmit data internationally. However, open-source systems of a similar nature have been developed by other organizations. A notable one is [Charity: Water](https://www.charitywater.org/), a non-profit serving several developing countries through community water projects and sanitation efforts. Their [sensors](https://www.charitywater.org/our-work/sensors)&mdash;which can be retrofitted onto the Afridev, Tapstand, and India Mark II hand pump models&mdash;are able to collect data on the quantity of water being pumped from a well. The gathered data is transmitted to the cloud weekly via cellular networks, where it is stored, processed, and made viewable by an application running on Amazon Web Services (AWS). The Afridev model appears to have been developed from 2016 to 2020 by Intelligent Product Solutions (IPS), while the India Mark II sensor was developed in 2021 by TwistThink over the course of at least a year.

Extensive documentation for the Afridev and India Mark II models of the sensor is available on [GitHub](https://github.com/charitywater/india-mark-ii-sensor). Charity: Water's third generation Afridev hand-pump sensor expands on its predecessors with the ability to gather GPS, temperature, and device performance information. A more recent model, the India Mark II sensor, adds to these capabilities with additional stroke and humidity sensors. It costs $250 to build and maintain for its battery-limited 10-year lifespan and can collect ten million data points over a 24-hour period. This data is continually analyzed through Charity: Water's AWS backend, where personalized emails are dispatched to monitoring teams daily. Design specifications for Charity: Water's sensors are available from open source and served as an initial reference for this design.

The Charity: Water sensors all use a similar strategy to measure water flow rate. During a pumping cycle, water is brought into a chamber where its depth is measured by a series of capacitive sensors. From this, the quantity per pump&mdash;and therefore average flow rate&mdash;can be accurately estimated. While functional, this method has a few weaknesses. The capacitive sensors are highly sensitive to environmental changes such as temperature variation and require frequent (multiple per hour) automated calibrations to maintain measurement accuracy. Moreover, the sensor has an upper limit on the amount of flow it can measure per pump&mdash;any output over the chamber volume cannot be measured. With this setup, Charity: Water claims a 20% tolerance for the calculated flow rate, which was improved to 5% by IPS. More information can be found [here](https://github.com/charitywater/afridev2-sensor/blob/master/software/architecture%20and%20analysis/charity%20water%20AfriDev%20sensor%20algorithm%20v.2.pdf). 

In addition to the common water sensing strategy, both sensors also feature a cellular module to communicate with Charity: Water's cloud infrastructure, GPS to provide positioning, and battery power of undisclosed capacity. It is claimed that the India Mark II sensor can remain operational for up to ten years. While cellular communication is available in some areas of the DRC, coverage is sparce outside of urban settlements. See the [network coverage maps](https://www.gsma.com/coverage/#88) for details. This makes the use of Global System for Mobile Communications (GSM) technology difficult to implement for the pump distribution in the DRC. Satellite communication is a more reliable alternative, with coverage in the DRC by telecommunications infrastructure providers such as Globalstar, Swarm, and Astrocast. For instance, [Globalstar's satellite networks](https://www.globalstar.com/en-us/products/iot/stx3) cover the entirety of the DRC, where this monitoring sensor can be implemented.