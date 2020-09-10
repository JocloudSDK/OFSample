package com.yy.orangefilter.test.adapter;

import android.view.View;
import android.widget.ImageView;

import com.yy.orangefilter.test.R;
import com.yy.orangefilter.test.base.BaseAdapter;
import com.yy.orangefilter.test.bean.FilterBean;

import org.jetbrains.annotations.NotNull;

import java.util.List;

/**
 * 滤镜
 */
public class FilterAdapter extends BaseAdapter<FilterBean> {

    private List<FilterBean> datas;

    public FilterAdapter(List<FilterBean> datas) {
        super(datas);
        this.datas = datas;
    }

    @Override
    public int getLayoutId(int viewType) {
        return R.layout.layout_menu_item;
    }

    @Override
    public void onCreateViewHolder(@NotNull VH holder, int viewType) {

    }

    @Override
    public void onBindViewHolder(@NotNull VH holder, FilterBean data, int position) {
        holder.setText(R.id.tvItem, data.getOptionName());
        holder.setImageRes(R.id.ivItem, data.getThumb());
        ImageView borderView = holder.getView(R.id.ivBorder);

        if (data.isEnable()) {
            borderView.setVisibility(View.VISIBLE);
        } else {
            borderView.setVisibility(View.INVISIBLE);
        }
    }

    public void selectItem(int position) {
        resetItem();
        datas.get(position).setEnable(true);
        notifyDataSetChanged();
    }

    public void resetItem() {
        for (int i = 0; i < datas.size(); i++) {
            datas.get(i).setEnable(false);
        }
    }


}
