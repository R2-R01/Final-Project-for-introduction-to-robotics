
function test_somethinga();
  
    % Make Pipeline object to manage streaming
    pipe = realsense.pipeline();
    % Make Colorizer object to prettify depth output
    colorizer = realsense.colorizer();
       imgstore=[];
    % Start streaming on an arbitrary camera with default settings
    profile = pipe.start();
figure('visible','on');  hold on;
    figure('units','normalized','outerposition',[0 0 1 1])
    % Get streaming device's name
    dev = profile.get_device();
    name = dev.get_info(realsense.camera_info.name);

    % Get frames. We discard the first couple to allow
    % the camera time to settle
    
    for i = 1:200
        
        fs = pipe.wait_for_frames();
            % Stop streaming

    % Select depth frame
    depth = fs.get_depth_frame();
    % Colorize depth frame
    color = colorizer.colorize(depth);

    % Get actual data and convert into a format imshow can use
    % (Color data arrives as [R, G, B, R, G, B, ...] vector)
    data = color.get_data();
    img = permute(reshape(data',[3,color.get_width(),color.get_height()]),[3 2 1]);
    img(:,1:100,:)=[];
    img(:,840:end,:)=[];
    img(460:480,:,:)=[];
    A(:,:,:)=img(:,:,:);
    % Display image
        g=rgb2gray(img);

size(g);

level=graythresh(g);

BW2 = edge(g,'Prewitt');

   Ibw1 = imfill(BW2,'holes');
binaryImage = bwareafilt(Ibw1, 1); % Extract largest blob only.


cleanedBinaryImage = bwareaopen(binaryImage, 15); % Remove blobs smaller than 40.

info = regionprops(cleanedBinaryImage,'Boundingbox');
for k = 1 : length(info)
     BB = info(k).BoundingBox;
     X1(:,k)=[BB(1)];
     X2(:,k)=[BB(2)];
     X3(:,k)=[BB(3)+4];
     X4(:,k)=[BB(4)+4];
     X=transpose([X1;X2;X3;X4]);

     
end

IExpandedBBoxes = insertShape(img,'Rectangle',X,'LineWidth',3);

overlapRatio = bboxOverlapRatio(X, X);
n = size(overlapRatio,1); 
overlapRatio(1:n+1:n^2) = 0;
g1 = graph(overlapRatio);
componentIndices = conncomp(g1);
c=1;
% Merge the boxes based on the minimum and maximum dimensions.
xmin = accumarray(componentIndices', X1, [], @min);
ymin = accumarray(componentIndices', X2, [], @min);
xmax = accumarray(componentIndices', X3, [], @max);
ymax = accumarray(componentIndices', X4, [], @max);

textBBoxes = [xmin ymin xmax ymax];

ITextRegion = insertShape(img, 'Rectangle', textBBoxes,'LineWidth',3);
 s(i,:)= [regionprops(ITextRegion,'centroid')];
xcenter=round(xmin +(xmax/2));
ycenter=round(ymin+(ymax/2));
cent=g(xcenter,ycenter);

str = strcat('<HI,1,2,',num2str(cent),',',num2str(xcenter),',',num2str(ycenter),'>')

imshow(ITextRegion)


    end
        pipe.stop();
        


end