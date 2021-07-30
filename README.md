# <font style="color:#4F46E5" > Saaf</font> water 

[![License](https://img.shields.io/badge/License-Apache2-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0) [![Community](https://img.shields.io/badge/Join-Community-blue)](https://developer.ibm.com/callforcode/get-started/) [![Website](https://img.shields.io/badge/View-Website-blue)](https://sample-project.s3-web.us-east.cloud-object-storage.appdomain.cloud/)

## Contents

- [Submission or project name](#submission-or-project-name)
  - [Contents](#contents)
  - [Short description](#short-description)
    - [What's the problem?](#whats-the-problem)
    - [How can technology help?](#how-can-technology-help)
    - [The idea](#the-idea)
  - [Demo video](#demo-video)
  - [The architecture](#the-architecture)
  - [Long description](#long-description)
  - [Project roadmap](#project-roadmap)
  - [Getting started](#getting-started)
  - [Live demo](#live-demo)
  - [Built with](#built-with)
  - [Contributing](#contributing)
  - [Team](#team)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Short description

### What's the problem?

Are you aware that almost 50% of the world’s population, including that of developed countries, is dependent on groundwater for drinking? 

Physical, Chemical, and Biological Contamination of groundwater have led to <em> 1 in 3 people globally not having access to safe drinking water, this is as per UNICEF and WHO report of 2019 </em>. Consumption of contaminated water leads to numerous health complications from dysentery to severe kidney damage.
The biggest problem is the lack of frequent monitoring and its communication about the Degradation of a Water source not just to the authorities but to the community too.  

### How can technology help?

With IoT devices, frequent water quality monitoring becomes automated and thanks to open-source web technologies, this information can be made accessible to every individual who is using a water source. Integration of IBM Cloud and Watson services enables us to not just inform about the degraded groundwater source but also informs the community about the purification methods whenever required. Understanding patterns from collected Data can be employed to reduce lab test dependency for approximate assessment. 

### The idea

<b><font style="color:#4F46E5" type="Roboto"> Saaf</font></b> <font type="Roboto"> water </font> is an end to end Open-source platform which when installed will:
1. Timely monitors groundwater and informs authorities as well as the community about water quality along with purification methods for consumption.
2. Predict when the water quality will go bad.
3. Reduce dependency on manual test kits or lab tests for approximate assessment.

## Demo video

[![Watch the video](./images/saaf-youtube.png)](https://youtu.be/_taOL857nPA)

## The architecture

![Saaf water Dashboard](./images/architecture.png)

1. The Saaf water hardware consists of a low power, cellular enabled setup that monitors water parameters like TDS, Turbidity, pH and more and sends it to IBM watson IoT platform.
2. The water parameter data is then collected by the NodeRed backed from the IBM Watson IoT platform and then stored in NoSQL database.
3. The Backend then sends the data via websocket to the Saaf water dashboard deployed on IBM Cloud Foundry.
4. Or users can get the groundwater quality information and purification methods via SMS too.

## Long description

[More detail is available here](./docs/DESCRIPTION.md)

## Project roadmap

The Saaf water currently does the following things.

- Monitors various groundwater parameters like TDS, pH, Turbidity, Electrical Conductivity. 
- Groundwater Quality Prediction.
- Informs authorities as well as community about water quality with purification methods.

![Roadmap](./images/tech_roadmap.png)
![Roadmap](./images/product_roadmap.png)

Note - Some things may take longer time because of COVID-19 restrictions in India.

## Getting started

In this section you add the instructions to run your project on your local machine for development and testing purposes. You can also add instructions on how to deploy the project in production.

- [sample-react-app](./sample-react-app/)
- [sample-angular-app](./sample-angular-app/)
- [Explore other projects](https://github.com/upkarlidder/ibmhacks)

## Live demo

You can find a running system to test at [callforcode.mybluemix.net](https://saaf-water.eu-gb.mybluemix.net/).

## Built with

- [IBM Node Red](https://nodered.org/docs/getting-started/ibmcloud#deploy-to-ibm-cloud) - Backend for saaf-Water
- [IBM Watson IOT Platform](https://www.ibm.com/cloud/watson-iot-platform) - The service which we use to manage hardware connections to IBM Cloud.
- [IBM Cloudant](https://www.ibm.com/cloud/cloudant) - The NoSQL database used
- [IBM Cloud Foundry](https://www.ibm.com/cloud/cloud-foundry) - The service which we used to deploy [Dashboard](https://saaf-water.eu-gb.mybluemix.net/)
- [Esri Arcgis Map](https://www.esri.com/en-us/arcgis/about-arcgis/overview) - Map we're using in [Dashboard](https://saaf-water.eu-gb.mybluemix.net/)
- [ReactJs](https://reactjs.org/) - The web framework used

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Team

![Authors](./images/team.png)
- [Hrishikesh Bhandari](https://github.com/Hrishikesh24)
- [Jay Aherkar](https://github.com/jehhhh)
- [Satyam Prakash](https://github.com/satyamprakash-iot)
- [Manikanta Chavvakula](https://github.comManikantaChavvakula7)
- [Sanket Marathe](https://github.com/msanket9)


## License

This project is licensed under the Apache 2 License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Based on [Billie Thompson's README template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2).
