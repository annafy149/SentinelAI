# SentinelAI
SentinelAI harnesses the power of artificial intelligence and a network of strategically placed surveillance cameras throughout the city to detect weapons in real-time.

##Inspiration
The inspiration for SentinelAI stems from the alarming rates of gun violence in the United States, which have reached unprecedented levels in recent years. Yesterday, the day we all sat down for the sprint of the hackathon, two mass shootings were happening in the span of an hour. With mass shootings becoming a growing concern, it has become increasingly clear that rapid response is a critical component in preventing further casualties and chaos. SentinelAI aims to bridge this gap by providing an advanced, AI-powered solution that can swiftly detect weapon draws and issue alerts to both authorities and users in the surrounding area. This real-time notification system empowers individuals to take immediate action, helping them reach safety and avoid danger.

##What it does
SentinelAI is an innovative solution designed to monitor and detect weapon draws in real-time using a combination of advanced computer vision technology and object detection through the YOLOv7 model. Deployed on an ESP32 camera, which simulates a real-life security camera, SentinelAI captures and processes video feeds to identify potential threats.

Upon detecting a weapon being drawn, the system instantly issues alerts to both the authorities and users of the SentinelAI app using both the app and Twilio. This rapid notification system enables users to take immediate action to find safety and avoid potentially dangerous situations. At the same time, the alerts provide law enforcement with crucial information, allowing them to respond quickly and effectively to the threat.

By harnessing the power of AI and computer vision, SentinelAI strives to create a safer environment, actively contributing to the mitigation of gun violence and the protection of communities.

##How we built it
We used an ESP32 board and an ESP32 cam to rapid-capture at a rate of 30 fps that is being transmitted into our local machine using the embedded TCP protocol. On the local machine, Matlab will be used to stitch together the frames for a consistent video. A custom YOLOv7 model that was trained on a dataset of 4,065 images and run through 100 epochs of training to maximize confidence and prevent overfitting will process the video feed in real-time and the if the model detects a weapon, it will immediately trigger an alert to the Firebase DB. Simultaneously, the system sends notifications to the authorities, enabling them to take immediate action.

We utilized the powerful Flutter framework to create a cross-platform app that works seamlessly on both iOS and Android devices. The app connects to Firebase, which serves as the backend infrastructure, allowing us to store and manage data effectively.

##Challenges we ran into
During the 24-hour hackathon, we faced several challenges while building SentinelAI, given the complexity and the numerous moving parts involved in the project.

Time constraint: With only 24 hours to complete the project, managing time efficiently while ensuring a high-quality end product was a significant challenge.
Integration: Integrating the various components, such as the YOLOv7 model, ESP32 camera, cloud services, and the Flutter app, required a deep understanding of each technology and seamless collaboration between team members.
Testing and debugging: Thoroughly testing and debugging the entire system, including the AI model, camera, cloud service, and app, under time pressure was challenging, as each component needed to work flawlessly to deliver the desired outcome.
##Accomplishments that we're proud of
We are proud of the things we achieved in the hackathon in a short time period.

We got the model to be trained well, keeping in mind the potential for the model to be overfitted. Figuring out the documentation for YOLOv7 is something we are proud of because the resources proved to be scarce.
Rapid development: Despite the limited timeframe, we managed to create a functional and well-integrated system that combined the YOLOv7 model, ESP32 camera, database services, and a cross-platform flutter app.

##What we learned
Neither of us had any experience programming with Flutter so both of us had to navigate the documentation and figure out how to create the app on top of that also learned how to integrate the FireBase database into the Flutter development environment.

We also learned a lot about Hardware integration like capturing the images using the ESP 32 cam and stitch together the images for a consistent video stream using MatLab.

##What's next for SentinelAI
We will continue to work on improving the Flutter App. We will also look into making hardware investments to create a complete prototype for a surveillance stream.
