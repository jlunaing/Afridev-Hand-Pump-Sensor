### Contents

1. [Design Development](#design-development)
2. [Overview of Solid Model](#overview-of-solid-model)
3. [Electronic Components Selection](#electronic-components-selection)
4. [System Integration and Assembly](#system-integration-and-assembly)
5. [Overview of Software](#overview-of-software)
6. [Manufacturing](#manufacturing)
7. [Further Development](#further-development)


## Design Development

For the initial decision and selection process, means of achieving each required function of the system were defined using a morphological chart (**Table 1**). This involved brainstorming ideas based on background research about technologies and known solutions.

<div align="center"><b>Table 1.</b> Morphological chart for design development.<br><br>

| Functions | Means |
|  :----:   | ----- |
| Measure flow rate out of the hand pump | <p align="left">- Monitor internal capacitance<br>- Flow meter/sensor<br>- Water wheel </p>|
| Convert mechanical energy to electrical energy | <p align="left">- Inductor array<br> - Magnets and coil </p>|
| Translate electrical energy | <p align="left">- Convert analog to digital<br>- Memory to transmit package<br> - Convert to correct units </p>|
| Transmit data | <p align="left">- Satellite<br>- Cellular </p>|
| Conserve battery life | <p align="left">- Solar panel<br> - Capacitor array </p>|
| Protect from external environment | <p align="left">- Epoxy resin enclosure<br>- Sensor box inserted inside pump<br>- Polycarbonate housing </p>|

</div>

The functions and means were gathered in [decision matrices](decision_matrices.md) to compare methods based on different criteria. From these matrices, we selected the optimal method for achieving each function and brainstormed ways to combine these components. The different key components of our design include the power management, processing, system enclosure, pump functionality sensing, and wireless communication.

Regarding the system enclosure, an early design considered aluminum as a  suitable option based on durability, waterproofing, and environmental resistance to factors such as UV exposure. Upon further analysis and conceptual integration, we realized that aluminum would inhibit the ability of the system to transmit data. Furthermore, the thermal properties of aluminum could foster an internal temperature inside the enclosure that may not be ideal for component operations. High temperatures within the enclosure and pump could also facilitate evaporation of water inside the pump, resulting in condensation and potential damage within the circuitry. As such, the final version uses 3D-printed polycarbonate and epoxy seal to ensure that the electronics are both permanently protected and able to transmit data without an external antenna.

For power generation and storage, we paired a solar panel and lithium-ion batteries to harvest natural energy and store it within the battery. Power management is driven by the integration of low-power devices and using a sleep/idle mode on the system. This way, the limited power from solar energy can be preserved for longer. A modification from the initial design involved switching from the single 18650-sized battery to three small 10440 AAA-sized batteries connected in parallel to minimize the final package height.

To transmit data wirelessly, we determined satellite to be a feasible option in our region of operation within the DRC, due to the lack of cellular coverage in the Équateur Province. Initially, we selected SWARM as a satellite provider for their pricing and reliable coverage. However, their modem requires a large antenna to communicate with the satellite, which puts the functionality of the device at risk if the antenna were to be damaged or stolen. We aimed to find a new satellite device and provider that offered a small and subtle design that can be fully enclosed within the system. This necessitated a switch to a higher-frequency network that could be used with a smaller antenna. We settled on GlobalStar as a provider, which operates its network at a frequency of $1.6$ GHz. Their STX3 patch antennae integrates into the final, lower-profile design and can transmit data to the United States using satellite.

## Overview of Solid Model

**Figure 1-2** illustrates the final iteration of the system design. It features a printed-circuit board inside a polycarbonate housing. The system was designed with subtlety in mind, so it features a low-profile exterior that protects the electronics inside. The unit is securely fastened to the top panel of the Afridev hand pump with fasteners that are hidden from external view. At the bottom surface, the enclosure has dimensions of $90 \times 115$ mm and is attached to the top cover of the hand pump, directly over the axis of motion of the plunger, allowing downward clearance for both the time-of-flight and the infrared sensors.

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/7f0130a3bdbf5bf57193291cfedf667f1ba475b3/docs/img/fig1.png" 
   width=60%    
   alt="Figure 1"/>
</p>

<p align="center"><b>Figure 1.</b> Monitoring system installed on top of the Afridev hand pump.</p>

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig2.png" 
   width=70%    
   alt="Figure 2"/>
</p>

<p align="center"><b>Figure 2.</b> Close-up view of monitoring system.</p>

A detailed solid of the polycarbonate enclosure can be found in the [hardware](hardware) folder, where version [v2](hardware/v2) corresponds to the latest design. The mechanical design of the housing features walls at $60\degree$ angles to dissuade removal of the sensor. The electrical system consists of batteries, a solar cell, charge controller, two sensors, a satellite modem, and a microcontroller. These components, as described in the following sections, are integrated into a PCB which is situated between the bottom and top housing. An exploded and collapsed view of the housing and the PCB is shown below (**Figure 3-4**). Detailed engineering drawings and the detailed electrical circuit schematic can be found in the [Drawings](hardware/v2/Drawings) and [PCB](pcb) folders. The software system that the design will use to process the data is described in the software development plan in the [Product Realization](#product-realization) section.

<p align="middle">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig3.png" 
   width=50%    
   alt="Figure 3"/>
</p>

<p align="center"><b>Figure 3.</b> Exploded view of monitoring system. Colors shown for visual purposes.</p><br>

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig4.png" 
   width=60%    
   alt="Figure 4"/>
</p>

<p align="center"><b>Figure 4.</b> "Collapsed" exploded view of monitoring system</p>

## Electronic Components Selection

The components can be selected based on constraints and factors including cost, performance metrics, compatibility and others described earlier in the design development section. To provide power to the sensor system, three 10440 AAA rechargeable lithium-ion phosphate batteries, also known as LiFePO4 batteries, were selected. Compared to traditional alkaline AAA batteries, 10440 LiFePO4 batteries have a higher voltage, energy density, and are rechargeable so they can provide more power and last longer. They also have a more stable chemistry than other lithium-ion alternatives, lasting up to 10 years. Connected in parallel, these batteries will supply $3.2$ V of power with a combined capacity of $1200$ mAh. The LiFePO4 is a readily available battery, often used in solar garden lights and similar home applications. Since they will be connected in parallel, charging is balanced between the two batteries. 

The design also incorporates the [SM141K06L](https://www.digikey.com/en/products/detail/anysolar-ltd/SM940K12L/9990451) monocrystalline solar cell, which will recharge the batteries from solar energy. This specific cell provides a $4.15$-V nominal output with 183-mW peak power. It comes pre-sealed and has a size of $35\times22$ mm, allowing for ease of integration. To incorporate the solar cell and batteries, we use a shunt charger. This is the simplest type of charger and can be used in this application because the capacity of the batteries far outclasses the maximum output of the solar cell. The solar cell is directly connected to the batteries, and a shunt circuit limits voltage to the battery’s max&mdash;about $3.6$ V. When the batteries are full, additional power generated by the solar cell is simply wasted as heat through the shunt to keep the battery voltage below its max. The maximum power point (MPP) of the solar cell is specifically selected so that it is near the target voltage of the battery, ensuring efficient power extraction. When charging at around $3.5$ V, the panel will sit at about $3.7$ V after a reverse-bias diode, which is near enough the MPP for efficient operation.

To ensure that the handpump system is working, two sensors are responsible for measurements&mdash;specifically, detecting physical pumping action and the presence of water. The [VL53L4CD](https://www.digikey.com/en/products/detail/adafruit-industries-llc/5425/16499347) time-of-flight sensor from ST Microelectronics measures the position of the pump plunger as it moves vertically during pumping. From this displacement and the known cross-sectional area of the plunger, water outflow can be estimated. The sensor includes some useful features such as adjustable distance threshold interrupts that can be used to optimize the power draw of the microcontroller. It also features an accuracy of about $5$ mm. This component runs on $3.3$ V, with a current draw of $19$ mA during active mode and $5$ $\mu$A in idle mode.

Likewise, to detect whether water is indeed flowing when pumping action occurs, we selected the [MLX90614](https://www.digikey.com/en/products/detail/melexis-technologies-nv/MLX90614ESF-BAA-000-SP/5414793) digital infrared temperature sensor from Melexis. By measuring the temperature of a region of the pump that becomes flooded when water is flowing, a temperature drop can be used to directly check that water is present. Its measurement resolution, accuracy, and wide range make it suitable for our purposes. It requires a power supply in the range $2.6$-$3.6$ V to measure temperatures from -$40$ to $85\degree$C.

To transmit data back to CLEAN, we chose the [STX3](https://www.globalstar.com/en-us/products/iot/stx3) satellite modem from Globalstar. It relies on a low-profile, $20\times20$-mm patch antenna that is mounted directly to the PCB together with a surrounding $60\times60$-mm ground plane to function. Globalstar's network covers the entirety of the DRC and operates at an L-band of $1.6$ GHz. At the time of writing this report, each STX3 Modem comes at an initial cost of $60, with a subscription of $16 per month for data transmission.

To interface with the sensor and satellite module as well as control the overall behavior of the electronic system, we chose the ESP32-S2 microcontroller (MCU). The ESP32 series of microcontrollers is highly integrated, ubiquitous, and features built-in Wi-Fi. The [S2 variant](https://www.espressif.com/sites/default/files/documentation/esp32-s2_technical_reference_manual_en.pdf#sysmem) also features an ultra-low-power RISC-V co-processor which can be used to run arbitrary code while the main, power "hungry", MCU is in deep-sleep. This co-processor has low power consumption at $627$ $\mu$W and&mdash;important for this application&mdash;has access to the I2C peripheral. This enables it to communicate with the various sensors without booting the main MCU. The co-processor also has access to $8$ kB of SRAM, which is sufficient for the outlined requirements. The primary processor has $1$ MB of flash and $320$ kB of SRAM [22]. The primary processor has access to additional UART peripherals, which are used to communicate with the satellite transceiver. This system uses the C programming language for developing the code. It integrates into our compact design with dimensions of 18x25mm [16]. The extremely rich feature-set of the ESP32-S2 also keeps doors open to additional features like a smartphone-enabled setup.

The [MAX77827](https://www.mouser.com/new/analog-devices/maxim-max77827-switch-buckboost-converter/) buck-boost converter IC was selected to regulate the main $3.3$-V rail at an efficiency of $96$ %. It accepts input voltages in the range of $2.3$-$5.5$ V and can output a steady $3.3$ V, regardless of whether the input voltage is above or below the target. This is important for functionality, as during satellite transmission, peak current draws may cause battery voltages to dip below $3.3$ V, making a standard step-down only converter inadequate.

To monitor battery voltage, the system uses the [MCP3425A0T](https://www.digikey.com/en/products/detail/microchip-technology/MCP3425A0T-E-CH/1827893) ADC. This component supports I2C serial interface and runs within the range of power supplied by the previously mentioned buck converter. It has up to $16$-bit resolution in a SOT-23-6 package and can use its internal $2$-V reference to sense the entire battery range without needing a voltage divider. An external ADC is used instead of the ESP32’s built-in ADC to manage quiescent current. The ESP32’s ADC is notoriously poor and has a relatively low impedance&mdash;it will waste power if used.

To preserve the Lithium-ion batteries, the [XB8358D0](https://datasheet.lcsc.com/lcsc/1809131533_XySemi-XB8358D0_C80218.pdf) has been integrated. This battery management IC is contained in a small SOT23-5 package and runs on low current, switching between operation mode ($2.8$ $\mu$A) and power-down mode ($1.5$ $\mu$A). This IC will extend battery life by protecting against overcharging, over discharging, overcurrent, load short circuiting, and reverse polarity. While not strictly necessary if the batteries are never replaced, it is included under the assumption that batteries should be serviceable by laypeople.

Additional components, included only for testing and validation on the prototype, include the CP2102 USB-serial converter, the BQ25170DSGR Li-ion charger, and the LM66200DRLR power switch. These components should not be populated for production versions of the device.

## System Integration and Assembly
 
The PCB is mounted onto the bottom part of the housing, where an electrical potting compound is used to permanently the bottom side of the PCB. The two holes surrounding the time-of-flight and infrared sensors are designed to prevent the potting epoxy from leaking onto the sensors and causing issues (**Figure 5**). 

[Epoxy Seal 9000](https://www.theepoxyresinstore.com/products/epoxyseal-9000-2-to-1-electronic-grade-potting-epoxy-resin-48oz-kit) has high temperature and impact resistance and keeps moisture out of the enclosure. This product is RoHS compliant and can be purchased online for $0.08 per fluid ounce, at the time of writing this report. Potting the important components secures the design against component theft and water damage.

The solar panel is mounted to the top part of the housing, with the perforations beneath it designed to allow epoxy to hold it in place effectively (**Figure 6**). This part of the housing has six holes in which metal heat-set inserts can be placed for later attachment to the pump.

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig5.png" 
   width=60%    
   alt="Figure 5"/>
</p>

<p align="center"><b>Figure 5.</b> The PCB and associated part of the enclosure, with sensor holes highlighted.</p><br>

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig6.png" 
   width=60%    
   alt="Figure 6"/>
</p>

<p align="center"><b>Figure 6.</b> The solar panel and its associated perforations.</p>

To assemble the system onto the hand pump, we first drill six holes through the top cover of the hand pump according to the [drawings](hardware/v2/Drawings). These holes match the position of the heat-set inserts on the top part of the enclosure.

Then, the joint assembly of the bottom enclosure with the PCB are placed on top of the pump with its edges inside the perimeter created by the position of the holes on the cover plate of the pump. The solar panel, sealed on top of the top enclosure is connected through its terminals to the PCB. The top enclosure is lowered and mates with the bottom enclosure. Finally, the this assembly is fastened to the cover of the hand pump using M3 screws and the heat-set inserts already on the enclosure. Epoxy seal was not used for the prototype developed to maintain accessibility and viewability of the design.

Combined with slightly undersized holes, these heat-set inserts produce a locking effect like nylon locknuts. The trade-off for the added security from potting and locking screws is that only the replacement of the solar panel and batteries is possible. Therefore, extensive testing should be completed to ensure longevity of each component and the whole system, which would be deployed as a single unit that requires replacement approximately every 10 years. 

## Overview of Software

The software required to create a functioning system needs to be able to perform data collection, translation, and transmission, as well as transitioning a system between idle and active mode. The software is programmed and processed on a microcontroller unit (MCU) that acts as the central controller for the entire system. The finite state diagram for the software is shown in **Figure 7**.

<p align="center">
   <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig7.png" 
   width=70%    
   alt="Figure 7"/>
</p>

<p align="center"><b>Figure 7.</b> Finite state machine driving the microcontroller unit.</p>

For data collection, the temperature (IR) and distance (ToF) sensor work in tandem to detect if the pump is working. To begin data collection, the ToF sensor is first used to detect if the handle is being pumped. This communicates with the MCU using the I2C protocol and provides accurate measurements. When the sensor reads that the pump is pulled up, the sensor triggers an interrupt (INTR in diagram) in the MCU to come out of [standby mode](https://www.st.com/resource/en/user_manual/um2923-a-guide-to-using-the-vl53l4cx-timeofflight-sensor-with-extended-distance-measurement-stmicroelectronics.pdf). The MCU then turns on the IR sensor, which takes measurements on the ambient and object temperature. The target of interest for the IR sensor is the bottom of the tank where water starts collecting, and the measured data is the temperature difference caused by the presence of water within the tank of the hand pump, relative to the temperature of the tank when no water is present, as described earlier. The MCU makes use of a timer, that is set when the interrupt is triggered, to minimize the data collection when the pump is not being used. 

For data translation, the system must be able to translate measured data values into volumetric measurements of water being pumped. The MCU uses the displacement of the pump handle mechanism to estimate the volume of water displaced by the plunger. This relationship can be determined empirically from testing with the Afridev pump. The IR sensor determines if water is present at the time of pumping. These measurements are calculated and translated to the number of liters output by the pump and detect if the pump has failed or not.

For data transmission, the MCU uses satellite to send the data internationally. The MCU prepares the data packets to send, which may include the pump number, failure status, a checksum, and number of liters pumped over the course of a day. The pump number is a numerical representation of a pump station that can be mapped to the physical location of the pump. The failure status displays if a pump has failed or is still active. The checksum is a calculated value that is sent with the data packet to be checked at the receiving end, to ensure that data has not been corrupted during transmission. The number of liters can be stored and displayed within an  external database.  

## Manufacturing

In regard to the PCB, most components were pre-populated on the board from the PCB manufacturer's assembly service. This is convenient as it saves time and money&mdash;the components they have open for selection are often orders of magnitude cheaper than what can be found on popular supplier sites. Components that could not be found in their available library for assembly were ordered from Mouser and Digikey then soldered by hand. The demonstration enclosure was printed on a group member’s 3D printer out of PETG. 

<p align="middle">
  <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig9.jpg" width=40% />
  <img src="https://github.com/jlunaing/Afridev-Hand-Pump-Sensor/blob/937f5e11812a0062556b8e444fde13d92a296132/docs/img/fig9b.jpg" width=40% /> 
</p>

<p align="center"><b>Figure 9.</b> (a) Model Afridev hand pump on stand. (b) Open sensor prototype on pump.</p>

## Further Development

These are some recommendations for further developing this project. There are areas where costs can be potentially cut, if desired. In particular, the MLX90614 temperature sensor&mdash; which represents a significant cost&mdash;can likely be replaced with a standard Passive Infrared (PIR) element like those from Zilog. This is possible because it only needs to effectively detect the temperature difference caused by flowing water, not the temperature itself. Additionally, the satellite transceiver should, once coverage expands to the USA in 2024, likely be replaced with the Astronode series of transceivers from Astrocast instead of the chosen Globalstar counterparts. These modules are both more diverse in features and cheaper to buy and operate than the Globalstar offering integrated into the prototype.